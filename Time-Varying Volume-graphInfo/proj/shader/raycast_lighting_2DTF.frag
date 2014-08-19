#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect texStartPos;
uniform sampler2DRect texEndPos;
uniform sampler2DRect texTransfunc;  //none
uniform sampler3D texVolume;  //none
uniform sampler3D texGradient;
uniform sampler3D texV1; //none
uniform sampler3D texV2;                 //feature

uniform sampler1D tex1DTF;

uniform float step;
uniform float ka;
uniform float kd;
uniform float ks;
uniform float shininess;
uniform vec3 lightPos;
uniform vec3 viewDir;

vec3 PhongShading(vec3 samplePos, vec3 diffuseColor)
{
	vec3 shadedValue=vec3(0,0,0);

	//�����ݶ�
// 	vec3 N;
// 	N.x = (texture3D(texVolume, samplePos+vec3(step,0,0) ).w - texture3D(texVolume, samplePos+vec3(-step,0,0) ).w) - 1.0;
// 	N.y = (texture3D(texVolume, samplePos+vec3(0,step,0) ).w - texture3D(texVolume, samplePos+vec3(0,-step,0) ).w) - 1.0;
// 	N.z = (texture3D(texVolume, samplePos+vec3(0,0,step) ).w - texture3D(texVolume, samplePos+vec3(0,0,-step) ).w) - 1.0;

	vec3 N = texture3D(texGradient, samplePos).xyz;
 	N = N * 2.0 - 1.0;
  	N = -normalize(N);

	vec3 L = lightPos;
	vec3 V = viewDir;

	//specularcolor
	vec3 H = normalize(V+L);
	float NdotH = pow(max(dot(N, H), 0.0), shininess);
	float NdotL = max(dot(N, L), 0.0);
	
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
	//vec4 bg = vec4(1.0, 1.0, 1.0, 1.0);

    if (start2end.x == 0.0 && start2end.y == 0.0 && start2end.z == 0.0) {
		gl_FragColor = bg; // Background Colors
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
        vec4 scalar = texture3D(texVolume, samplePoint);

		//float v1 = texture3D(texV1, samplePoint).r;
		//float v2 = texture3D(texV2, samplePoint).r;

		 vec4 sampledColor = texture1D(tex1DTF, scalar.r);

		//vec4 sampledColor = texture2DRect(texTransfunc, vec2(v1*255.0, v2*255.0));
		sampledColor.rgb = PhongShading(samplePoint, sampledColor.rgb);
		
        color = color + sampledColor * vec4(sampledColor.aaa, 1.0) * (1.0 - color.a);
        if(color.a > 0.99)
            break;
    }

	color = color + vec4(bg.rgb, 0.0) * (1.0 - color.a);
    color.a = 1.0;
    gl_FragColor = color;
}
