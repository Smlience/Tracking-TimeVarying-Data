#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect texStartPos;
uniform sampler2DRect texEndPos;
uniform sampler3D texVolume;
uniform sampler3D texGrad;
uniform sampler3D texGM;
uniform float step;
uniform float exponent;

float CDGDotProduct(vec3 g0, vec3 g1)
{
	return 1.0 - dot(normalize(g0), normalize(g1));
}

vec3 PhongShading(vec3 samplePos, vec3 diffuseColor)
{
    vec3 shadedValue=vec3(0,0,0);

    vec3 N = texture3D(texGrad, samplePos).xyz;
    N = normalize(N * 2.0 - 1.0);
	const float ka = 0.3;
	const float kd = 1.0;
	const float shininess = 32.0;
	const float ks = 0.7;
//	vec3 V = -vec3(0.0, 0.0, 1.0);
//	vec3 L = normalize((vec4(lightPos.xyz, 1.0) * gl_ModelViewMatrix).xyz);
	vec3 V = -(vec4(0.0, 0.0, 1.0, 1.0) * gl_ModelViewMatrix).xyz;
	vec3 L = V;
// 	vec3 L = lightPos;
// 	vec3 V = viewDir;

    //specularcolor
    vec3 H = normalize(V + L);

	float NdotL = dot(N, L);
	if(NdotL < 0.0){
		N = -N;
		NdotL = -NdotL;
	}
    float NdotH = pow(max(dot(N, H), 0.0), shininess);

    vec3 ambient = ka * diffuseColor.rgb;
    vec3 specular = ks * NdotH * vec3(1.0,1.0,1.0);
    vec3 diffuse = kd * NdotL * diffuseColor.rgb;

    shadedValue = specular + diffuse + ambient;
    return shadedValue;
}

void main()
{
	vec3 rayStart = texture2DRect(texStartPos, gl_TexCoord[0].st).xyz;
    vec3 rayEnd = texture2DRect(texEndPos, gl_TexCoord[0].st).xyz;
    vec3 start2end = rayEnd - rayStart;
    vec4 bg = vec4(0.156863, 0.156863, 0.156863, 1.0);
	//vec4 bg = vec4(0.0, 0.0, 0.0, 1.0);
    if (start2end.x == 0.0 && start2end.y == 0.0 && start2end.z == 0.0) {
        gl_FragColor = bg; // Background Color
        return;
    }
    vec4 color = vec4(0, 0, 0, 0);
    vec3 direction = normalize(start2end);
    float distance = dot(direction, start2end);
    int steps = int(distance / step);
    for(int i = 0; i < 2048; ++i) {
	    if(i >= steps)
	        break;
        vec3 samplePoint  = rayStart + direction * step * (float(i) + 0.5);
        vec4 sampleColor = texture3D(texVolume, samplePoint).xyzw;

// 		vec3 g0 = texture3D(texGrad, samplePoint).xyz;
// 		vec3 g1 = texture3D(texGrad, vec3(samplePoint.x - 1.0, samplePoint.yz)).xyz;
// 		vec3 g2 = texture3D(texGrad, vec3(samplePoint.x + 1.0, samplePoint.yz)).xyz;
// 		vec3 g3 = texture3D(texGrad, vec3(samplePoint.x,samplePoint.y - 1.0, samplePoint.z)).xyz;
// 		vec3 g4 = texture3D(texGrad, vec3(samplePoint.x,samplePoint.y + 1.0, samplePoint.z)).xyz;
// 		vec3 g5 = texture3D(texGrad, vec3(samplePoint.xy,samplePoint.z - 1.0)).xyz;
// 		vec3 g6 = texture3D(texGrad, vec3(samplePoint.xy,samplePoint.z + 1.0)).xyz;
// 		float opacity = max(CDGDotProduct(g0, g1), CDGDotProduct(g0, g2));
// 		opacity = max(opacity, CDGDotProduct(g0, g3));
// 		opacity = max(opacity, CDGDotProduct(g0, g4));
// 		opacity = max(opacity, CDGDotProduct(g0, g5));
// 		opacity = max(opacity, CDGDotProduct(g0, g6));
//		sampleColor.a = pow(opacity * 0.1, exponent);
		///////////////////////////////////////////
		// 不透明度的计算
 		float gm = texture3D(texGM, samplePoint).r / 255.0;
		sampleColor.a = pow(gm * 0.5, exponent);
		//gm = gm * 6;
		//sampleColor = vec4(gm, gm, gm, gm);
		///////////////////////////////////////////
		//sampleColor.rgb = PhongShading(samplePoint, sampleColor.rgb);
        color = color + sampleColor * vec4(sampleColor.aaa, 1.0) * (1.0 - color.a);
        if(color.a > 0.99)
            break;
    }
    
    color = color + vec4(bg.rgb, 0.0) * (1.0 - color.a);
    color.a = 1.0;
    gl_FragColor = color;
}