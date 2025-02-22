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
varying vec4 vPos;
uniform bool enableDirtMap;

varying float Blur;
uniform float focalDistance, focalRange;

void main()
{
	//normal calculations based on normal texture
    vec3 n, t, b;
	mat3 tbnMatrix;
	n = normalize(gl_NormalMatrix * gl_Normal);
	t = normalize(gl_NormalMatrix * gl_MultiTexCoord1.xyz);
	b = normalize(gl_NormalMatrix * gl_MultiTexCoord2.xyz);
		
	tbnMatrix = mat3(t.x, b.x, n.x,
                     t.y, b.y, n.y,
                     t.z, b.z, n.z);

    gl_TexCoord[0] = gl_MultiTexCoord0;
    if( enableDirtMap )
		gl_TexCoord[1] = gl_MultiTexCoord3; //second uv set

    vPos = gl_ModelViewMatrix * gl_Vertex;

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
uniform bool enableAlphaMap;
uniform bool enableColorMap;
uniform bool enableDirtMap;
uniform bool enableGlossMap;

uniform sampler2D colorMap;
uniform sampler2D alphaMap;
uniform sampler2D heightMap;
uniform sampler2D glossMap;
uniform sampler2D dirtMap;

//normal and parallax mapping
uniform sampler2D normalMap;
uniform bool enableParallaxMap;
uniform float parallaxMapScale;
uniform float parallaxMapBias;

varying float Blur;
uniform bool enableFog;

out vec4 colorOut;

//shadow
uniform vec4 far_d;
uniform float shadow_intensity;
varying vec4 vPos;
uniform sampler2DArrayShadow stex;

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
	vec2 newTexCoord;
  	vec3 halfV, viewV, ldir, ldirForDynamicShadow, n, sdir;
	vec4 ambient, diffuse, textureColor, finalColor, alphaColor, glossColor, dirtColor, color, ambientGlobal, specular;

	if( enableAlphaMap )
	{
	    alphaColor = texture2D(alphaMap, gl_TexCoord[0].xy);
	
	    if( alphaColor.a < 0.1)
	    	discard;
	}
	viewV = normalize( viewDir );

	if( enableParallaxMap )
	{
		//parallax mapping
		float height = texture2D(normalMap, gl_TexCoord[0].st).a;
        
		height = height * parallaxMapScale + parallaxMapBias;
		newTexCoord = gl_TexCoord[0].st + (height * viewV.xy) * texture2D(normalMap, gl_TexCoord[0].st ).z;
	}
	else 
		newTexCoord = gl_TexCoord[0].st;
		
    n = normalize(texture2D(normalMap, newTexCoord ).rgb * 2.0 - 1.0);

	int light_index = 0;
	//Directional Light
	for(int i = 0; i < nr_dir_lights; i++)
	{
		if( i == defaultDirLightIndex)
		{
			ambient = gl_FrontMaterial.ambient * gl_LightSource[light_index].ambient;
			diffuse = gl_FrontMaterial.diffuse * gl_LightSource[light_index].diffuse;

			color = ambient;

			ldir = normalize(lightDir[i]); 
			NdotL = max(dot(n,ldir),0.0);
			if( NdotL > 0.0 )
			{
				float shadow_coef = shadowCoef(NdotL);

				color += (diffuse * NdotL );
			
	 			specular = gl_FrontMaterial.specular * gl_LightSource[light_index].specular;
	
				float l_specular = pow(clamp(dot(reflect(-ldir, n), viewV), 0.0, 1.0), 
								gl_FrontMaterial.shininess );
	
				if( enableGlossMap )
				{
	 				glossColor = texture2D(glossMap, newTexCoord );
					color += specular * l_specular * glossColor;
				}
				else
					color += specular * l_specular;

				shadow_coef +=  1.0 - shadow_intensity;
	
				shadow_coef = clamp(shadow_coef, 0.0, 1.0);
				color *= shadow_coef;
			}
			else
			{
				color *= 1.0 - shadow_intensity;
			}
		}
		else
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
	
				if( enableGlossMap )
				{
	 				glossColor = texture2D(glossMap, newTexCoord );
					color += specular * l_specular * glossColor;
				}
				else
					color += specular * l_specular;
			}
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
	
			if( enableGlossMap )
			{
	 			glossColor = texture2D(glossMap, newTexCoord );
				color += att * specular * l_specular * glossColor;
			}
			else
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
	
				if( enableGlossMap )
				{
	 				glossColor = texture2D(glossMap, newTexCoord );
					color += att * specular * l_specular * glossColor;
				}
				else
					color += att * specular * l_specular;
			}
		}
	
		finalColor += color;
		light_index++;
	}	

	if( enableColorMap )
	{
  		textureColor = texture2D(colorMap, newTexCoord );
		finalColor *= textureColor;
	}

	if( enableDirtMap )
	{
  		dirtColor = texture2D(dirtMap, gl_TexCoord[1].st );
		finalColor *= dirtColor;
	}

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


