#extension GL_ARB_texture_rectangle : enable
uniform sampler1D texTransfunc;
uniform sampler2DRect texStartPos;
uniform sampler2DRect texEndPos;
uniform sampler3D texVolume;
uniform float step;

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
        vec4 scalar = texture3D(texVolume, samplePoint);
		///////////////////////////////////////////////////
		// 根据邻域获取聚类的三线性差值
// 		float cluster = scalar.r;
// 		float v000 = texture3D(texVolume, vec3(samplePoint.x - step, samplePoint.y - step, samplePoint.z - step)).r;
// 		float v001 = texture3D(texVolume, vec3(samplePoint.x - step, samplePoint.y - step, samplePoint.z + step)).r;
// 		float v010 = texture3D(texVolume, vec3(samplePoint.x - step, samplePoint.y + step, samplePoint.z - step)).r;
// 		float v011 = texture3D(texVolume, vec3(samplePoint.x - step, samplePoint.y + step, samplePoint.z + step)).r;
// 		float v100 = texture3D(texVolume, vec3(samplePoint.x + step, samplePoint.y - step, samplePoint.z - step)).r;
// 		float v101 = texture3D(texVolume, vec3(samplePoint.x + step, samplePoint.y - step, samplePoint.z + step)).r;
// 		float v110 = texture3D(texVolume, vec3(samplePoint.x + step, samplePoint.y + step, samplePoint.z - step)).r;
// 		float v111 = texture3D(texVolume, vec3(samplePoint.x + step, samplePoint.y + step, samplePoint.z + step)).r;
// 		cluster = (v000 + v001 + v010 + v011 + v100 + v101 + v110 + v111) / 8.0;
// 		vec4 sampledColor = texture1D(texTransfunc, cluster);
		///////////////////////////////////////////////////

		//根据聚类信息获取体素对应的颜色和不透明度
        vec4 sampledColor = texture1D(texTransfunc, scalar.r);
		//直接用原数据的颜色
		//sampledColor = vec4(scalar.rgb, sampledColor.a);
		//sampledColor = vec4(clusterInfo.rrr, sampledColor.a);

        color = color + sampledColor * vec4(sampledColor.aaa, 1.0) * (1.0 - color.a);
        if(color.a > 0.99)
            break;
    }
    
    color = color + vec4(bg.rgb, 0.0) * (1.0 - color.a);
    color.a = 1.0;
    gl_FragColor = color;
}