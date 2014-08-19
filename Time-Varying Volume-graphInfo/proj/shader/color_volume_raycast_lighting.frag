#extension GL_ARB_texture_rectangle : enable
uniform sampler1D texTransfunc;
uniform sampler2DRect texStartPos;
uniform sampler2DRect texEndPos;
uniform sampler3D texVolume;
uniform sampler3D texCluster;
uniform sampler3D texGradient;
uniform float step;
uniform float ka;
uniform float kd;
uniform float shininess;
uniform float ks;
uniform vec3 lightPos;
uniform vec3 viewDir;

vec3 PhongShading(vec3 samplePos, vec3 diffuseColor)
{
    vec3 shadedValue=vec3(0,0,0);

    vec3 N = texture3D(texGradient, samplePos).xyz;
    N = normalize(N * 2.0 - 1.0);

//	vec3 V = -vec3(0.0, 0.0, 1.0);
//	vec3 L = normalize((vec4(lightPos.xyz, 1.0) * gl_ModelViewMatrix).xyz);
//	vec3 V = -(vec4(0.0, 0.0, 1.0, 1.0) * gl_ModelViewMatrix).xyz;
//	vec3 L = V;
	vec3 L = lightPos;
	vec3 V = viewDir;

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

    if (start2end.x == 0.0 && start2end.y == 0.0 && start2end.z == 0.0)
    {
        gl_FragColor = bg; // Background Color
        return;
    }
    vec4 color = vec4(0, 0, 0, 0);
    vec3 direction = normalize(start2end);
    float distance = dot(direction, start2end);
    int steps = int(distance / step);
	vec4 clusterInfo = vec4(0, 0, 0, 0);
	vec4 sampledColor = vec4(0, 0, 0, 0);
	vec4 scalar = vec4(0, 0, 0, 0);
	vec3 samplePoint;
    for(int i = 0; i < 2048; ++i)
    {
        if(i >= steps)
            break;
        samplePoint  = rayStart + direction * step * (float(i) + 0.5);
        scalar  = texture3D(texVolume, samplePoint); // 从体数据中采样得到RGB

		//根据聚类信息获取体素对应的颜色和不透明度
        clusterInfo = texture3D(texCluster, scalar.bgr);	//根据采样点的RGB值获取其聚类ID
        sampledColor = texture1D(texTransfunc, clusterInfo.r);//根据聚类ID查TF

        //直接用原数据的颜色
        sampledColor = vec4(scalar.rgb, sampledColor.a);
        sampledColor.rgb = PhongShading(samplePoint, scalar.rgb);

        color = color + sampledColor * vec4(sampledColor.aaa, 1.0) * (1.0 - color.a);
        if(color.a > 0.99)
            break;
    }
    color = color + vec4(bg.rgb, 0.0) * (1.0 - color.a);
    color.a = 1.0;
    gl_FragColor = color;
}