//Copyright (C) 2024 Ehsan Kamrani
//This file is licensed and distributed under MIT license
[vert]

#version 130

//Directional Light
#define NR_DIR_LIGHTS 2
uniform int nr_dir_lights;
varying vec3 lightDir[NR_DIR_LIGHTS];

//Point Light
#define NR_POINT_LIGHTS 4    
uniform int nr_point_lights;
varying float pointLightDist[NR_POINT_LIGHTS];
varying vec3 pointLightPos[NR_POINT_LIGHTS];

//spot Lights
#define NR_SPOT_LIGHTS 2
uniform int nr_spot_lights;
varying float spotLightDist[NR_SPOT_LIGHTS];
varying vec3 spotLightPos[NR_SPOT_LIGHTS];
varying vec3 spotLightDir[NR_SPOT_LIGHTS];

varying vec3 viewDir;
varying vec3 normal;

varying float alpha1;
varying float alpha2;

varying vec4 vPos;

varying float Blur;
uniform float focalDistance, focalRange;

void main()
{
	//normal calculations based on normal texture
    vec3 n, t, b;
	mat3 tbnMatrix;
	n = normalize(gl_NormalMatrix * gl_Normal);
	t = normalize(gl_NormalMatrix * gl_MultiTexCoord4.xyz);
	b = normalize(gl_NormalMatrix * gl_MultiTexCoord5.xyz);
		
	tbnMatrix = mat3(t.x, b.x, n.x,
                     t.y, b.y, n.y,
                     t.z, b.z, n.z);

    gl_TexCoord[0] = gl_MultiTexCoord0; //bottom
    gl_TexCoord[1] = gl_MultiTexCoord1; //slope
    gl_TexCoord[2] = gl_MultiTexCoord2; //top
    gl_TexCoord[3] = gl_MultiTexCoord3; //shadow map

	alpha1 = gl_Color.r;
	alpha2 = gl_Color.a;

    vPos = gl_ModelViewMatrix * gl_Vertex;

	normal = vec3( normalize( gl_NormalMatrix * gl_Normal ) );
		
    viewDir = vec3(-vPos);
   	viewDir = tbnMatrix * viewDir;
 
	int light_index = 0;
 	vec3 tempVec;
	//directional Light
	for(int i = 0; i < nr_dir_lights; i++)
	{
		tempVec = gl_LightSource[light_index].position.xyz;
		lightDir[i] = normalize( tbnMatrix * tempVec );

		light_index++;
	}

	//Point Lights
	for(int i = 0; i < nr_point_lights; i++)
	{
		tempVec = (gl_LightSource[light_index].position.xyz - vPos.xyz);
		pointLightDist[i] = length(tempVec);
		pointLightPos[i] = normalize(tbnMatrix * tempVec);
		light_index++;
	}	
	
	//Spot Light
	for(int i = 0; i < nr_spot_lights; i++)
	{
		tempVec = (gl_LightSource[light_index].position.xyz - vPos.xyz);
		spotLightDist[i] = length(tempVec);
		spotLightPos[i] = normalize( tbnMatrix * tempVec );
		spotLightDir[i] = normalize( tbnMatrix * gl_LightSource[light_index].spotDirection );
		light_index++;
	}	

    // fix of the clipping bug for both Nvidia and ATi
    #ifdef __GLSL_CG_DATA_TYPES
    gl_ClipVertex = vPos;
    #endif

    //DOF data
    float distance = abs(-vPos.z - focalDistance);
    if( distance < focalRange )
    {
		Blur = 0.0;
	}
	else
	{
   		Blur = clamp( (distance - focalRange) / focalRange, 0.0, 1.0);
   	}

    gl_Position = gl_ProjectionMatrix * vPos;
}

[frag]

#version 130

#extension GL_EXT_texture_array : enable

//Directional Light
#define NR_DIR_LIGHTS 2
uniform int nr_dir_lights;
varying vec3 lightDir[NR_DIR_LIGHTS];
uniform int defaultDirLightIndex;

//Point Light
#define NR_POINT_LIGHTS 4    
uniform int nr_point_lights;
varying float pointLightDist[NR_POINT_LIGHTS];
varying vec3 pointLightPos[NR_POINT_LIGHTS];
uniform float point_light_radius[NR_POINT_LIGHTS];

//spot Lights
#define NR_SPOT_LIGHTS 2
uniform int nr_spot_lights;
varying float spotLightDist[NR_SPOT_LIGHTS];
varying vec3 spotLightPos[NR_SPOT_LIGHTS];
varying vec3 spotLightDir[NR_SPOT_LIGHTS];
uniform float spot_light_radius[NR_SPOT_LIGHTS];

varying vec3 viewDir;
varying vec3 normal;  //It's just used for per pixel lighting without normal map

uniform sampler2D image1;
uniform sampler2D image2;
uniform sampler2D image3;
uniform sampler2D image1Normal;
uniform sampler2D image2Normal;
uniform sampler2D image3Normal;

uniform sampler2D shadowMap;
varying float alpha1;
varying float alpha2;

varying float Blur;
uniform bool enableFog;

out vec4 colorOut;
//shadow
uniform vec4 far_d;
varying vec4 vPos;
uniform sampler2DArrayShadow stex;
uniform float shadow_intensity;

float shadowCoef(float ndotl)
{
	int index;
	float blend = 0.0;
	
	if(gl_FragCoord.z < far_d.x) {
		index = 0;
		blend = clamp( (gl_FragCoord.z - far_d.x * 0.995) * 200.0, 0.0, 1.0); 
    }
	else if(gl_FragCoord.z < far_d.y) {
		index = 1;
		blend = clamp( (gl_FragCoord.z - far_d.y * 0.995) * 200.0, 0.0, 1.0); 
	}
	else if(gl_FragCoord.z < far_d.z) {
		index = 2;
		blend = clamp( (gl_FragCoord.z - far_d.z * 0.995) * 200.0, 0.0, 1.0); 
	}
	else if( gl_FragCoord.z < far_d.w )
	{
		index = 3;
		blend = clamp( (gl_FragCoord.z - far_d.w * 0.995) * 200.0, 0.0, 1.0);
	}
	else
	{
		return 1.0;
	}
	
	// transform this fragment's position from view space to scaled light clip space
	// such that the xy coordinates are in [0;1]
	// note there is no need to divide by w for othogonal light sources
	vec4 shadow_coord = gl_TextureMatrix[index]*vPos;
	

	shadow_coord.w = shadow_coord.z;
	//variable bias
	float bias = 0.001*tan(acos(ndotl));
	bias = clamp(bias, 0,0.0025);

	shadow_coord.w -= bias;
	
	// tell glsl in which layer to do the look up
	shadow_coord.z = float(index);
	
	// get the shadow contribution
	float ret = shadow2DArray(stex, shadow_coord).x;
	
	if (blend > 0.0) {
	    shadow_coord = gl_TextureMatrix[index+1]*vPos;
	
	    shadow_coord.w = shadow_coord.z;
	    shadow_coord.z = float(index+1);
    	
	    ret = ret*(1.0-blend) + shadow2DArray(stex, shadow_coord).x*blend; 
	}
	 
	
	return ret;
}

void main()
{
	float NdotL, att, spotEffect;
  	vec3 halfV, viewV, ldir, ldirForDynamicShadow, n, sdir;
	vec4 ambient, diffuse, textureColor, finalColor, glossColor, color, ambientGlobal, specular;

	vec4 image1NormalColor = texture2D(image1Normal, gl_TexCoord[0].xy);
	vec4 image2NormalColor = texture2D(image2Normal, gl_TexCoord[1].xy);
	vec4 image3NormalColor = texture2D(image3Normal, gl_TexCoord[2].xy);

	vec4 normalMapColor;
	normalMapColor = mix(image2NormalColor, image1NormalColor, alpha1);
	normalMapColor = mix(image3NormalColor, normalMapColor, alpha2);	

	//n = normalize( normal );
    n = normalize(normalMapColor.rgb * 2.0 - 1.0);
	viewV = normalize( viewDir );

	int light_index = 0;
	//Directional Light
	for(int i = 0; i < nr_dir_lights; i++)
	{
		ambient = gl_FrontMaterial.ambient * gl_LightSource[light_index].ambient;
		diffuse = gl_FrontMaterial.diffuse * gl_LightSource[light_index].diffuse;

		color = ambient;

		ldir = normalize(lightDir[i]); 
		NdotL = max(dot(n,ldir),0.0);
		if( NdotL > 0.0 )
		{

			color += (diffuse * NdotL );
			
	 		specular = gl_FrontMaterial.specular * gl_LightSource[light_index].specular;
	
			float l_specular = pow(clamp(dot(reflect(-ldir, n), viewV), 0.0, 1.0), 
						 gl_FrontMaterial.shininess );
	
			color += specular * l_specular;
		}
		//shadow
		if(i == defaultDirLightIndex)
		{
			float shadow_coef = shadowCoef(NdotL);
			shadow_coef +=  1.0 - shadow_intensity;
	
			shadow_coef = clamp(shadow_coef, 0.0, 1.0);
			color *= shadow_coef;
		}

		finalColor += color;
		light_index++;
	}

	//point lights
	for(int i = 0; i < nr_point_lights; i++)
	{
		color = vec4(0.0);

		ldir = normalize(pointLightPos[i]); 
		NdotL = max(dot(n,ldir),0.0);
		if( NdotL > 0.0 )
		{
			ambient = gl_FrontMaterial.ambient * gl_LightSource[light_index].ambient;
			diffuse = gl_FrontMaterial.diffuse * gl_LightSource[light_index].diffuse;

			att = ( 1 - ( pointLightDist[i] / point_light_radius[i] ) ) / gl_LightSource[light_index].constantAttenuation;
			att = max(att, 0.0);
			color = att * ambient;

			color += att * (diffuse * NdotL );
			
	 		specular = gl_FrontMaterial.specular * gl_LightSource[light_index].specular;
	
			float l_specular = pow(clamp(dot(reflect(-ldir, n), viewV), 0.0, 1.0), 
						 gl_FrontMaterial.shininess );
	
			color += att * specular * l_specular;
		}
	
		finalColor += color;
		light_index++;
	}

	//Spot Lights
	for(int i = 0; i < nr_spot_lights; i++)
	{
		color = vec4(0.0);
		ldir = normalize(spotLightPos[i]); 
		sdir = normalize(spotLightDir[i]);
		NdotL = max(dot(n,ldir),0.0);
		if( NdotL > 0.0 )
		{
			spotEffect = dot(sdir, -ldir);
			if (spotEffect > gl_LightSource[light_index].spotCosCutoff)
			{
				ambient = gl_FrontMaterial.ambient * gl_LightSource[light_index].ambient;
				diffuse = gl_FrontMaterial.diffuse * gl_LightSource[light_index].diffuse;

				spotEffect = pow(spotEffect, gl_LightSource[light_index].spotExponent);
				float tempAtt = ( 1 - ( spotLightDist[i] /  spot_light_radius[i] ) ) / gl_LightSource[light_index].constantAttenuation;
				att = spotEffect * tempAtt;

				color = att * ambient;

				color += att * (diffuse * NdotL );
			
		 		specular = gl_FrontMaterial.specular * gl_LightSource[light_index].specular;
	
				float l_specular = pow(clamp(dot(reflect(-ldir, n), viewV), 0.0, 1.0), 
							gl_FrontMaterial.shininess );
	
				color += att * specular * l_specular;

			}
		}
	
		finalColor += color;
		light_index++;
	}	

	vec4 image1Color = texture2D(image1, gl_TexCoord[0].xy);
	vec4 image2Color = texture2D(image2, gl_TexCoord[1].xy);
	vec4 image3Color = texture2D(image3, gl_TexCoord[2].xy);
	vec4 shadowMapColor = texture2D(shadowMap, gl_TexCoord[3].xy);

	vec4 color1 = finalColor * image1Color;
	vec4 color2 = finalColor * image2Color;
	vec4 color3 = finalColor * image3Color;

	finalColor = mix(color2, color1, alpha1);
	finalColor = mix(color3, finalColor, alpha2);	

	if( enableFog )
	{
		//Compute the fog here
		const float LOG2 = 1.442695; 
		float z = gl_FragCoord.z / gl_FragCoord.w;
		float fogFactor = exp2( -gl_Fog.density * 
			   gl_Fog.density * 
			   z * 
			   z * 
			   LOG2 );
		fogFactor = clamp(fogFactor, 0.0, 1.0);
	
		colorOut = vec4(mix(gl_Fog.color, finalColor, fogFactor ).rgb, Blur);
	}
	else
		colorOut = vec4(finalColor.r, finalColor.g, finalColor.b, Blur);
}


