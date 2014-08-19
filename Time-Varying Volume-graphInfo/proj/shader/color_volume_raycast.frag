#extension GL_ARB_texture_rectangle : enable
uniform sampler1D texTransfunc;
uniform sampler2DRect texStartPos;
uniform sampler2DRect texEndPos;
uniform sampler3D texVolume;
uniform sampler3D texCluster;
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
        vec4 scalar = texture3D(texVolume, samplePoint).xyzw;

		//根据聚类信息获取体素对应的颜色和不透明度
		vec4 clusterInfo = texture3D(texCluster, scalar.bgr);
		vec4 sampledColor = texture1D(texTransfunc, clusterInfo.r);

		//直接用原数据的颜色
		sampledColor = vec4(scalar.rgb, sampledColor.a);
		//sampledColor = vec4(clusterInfo.rrr, sampledColor.a);

		///////////////////////////////////////////////////
		// 对TF进行三线性差值
// 		vec3 xyz = samplePoint * 255.0;
// 		vec3 xyz0 = vec3(floor(xyz.x), floor(xyz.y), floor(xyz.z));
// 		vec3 xyz1 = vec3(ceil(xyz.x), ceil(xyz.y), ceil(xyz.z));
// 		vec3 xyzd0 = xyz - xyz0;
// 		vec3 xyzd1 = vec3(1.0 - xyzd0.x, 1.0 - xyzd0.y, 1.0 - xyzd0.z);
// 
// 		xyz0 /= 255.0;
// 		xyz1 /= 255.0;
// 		vec4 s000 = texture3D(texVolume, vec3(xyz0.x, xyz0.y, xyz0.z));
// 		vec4 s001 = texture3D(texVolume, vec3(xyz0.x, xyz0.y, xyz1.z));
// 		vec4 s010 = texture3D(texVolume, vec3(xyz0.x, xyz1.y, xyz0.z));
// 		vec4 s011 = texture3D(texVolume, vec3(xyz0.x, xyz1.y, xyz1.z));
// 		vec4 s100 = texture3D(texVolume, vec3(xyz1.x, xyz0.y, xyz0.z));
// 		vec4 s101 = texture3D(texVolume, vec3(xyz1.x, xyz0.y, xyz1.z));
// 		vec4 s110 = texture3D(texVolume, vec3(xyz1.x, xyz1.y, xyz0.z));
// 		vec4 s111 = texture3D(texVolume, vec3(xyz1.x, xyz1.y, xyz1.z));
// 		float c000 = texture3D(texCluster, s000.bgr).r;
// 		float c001 = texture3D(texCluster, s001.bgr).r;
// 		float c010 = texture3D(texCluster, s010.bgr).r;
// 		float c011 = texture3D(texCluster, s011.bgr).r;
// 		float c100 = texture3D(texCluster, s100.bgr).r;
// 		float c101 = texture3D(texCluster, s101.bgr).r;
// 		float c110 = texture3D(texCluster, s110.bgr).r;
// 		float c111 = texture3D(texCluster, s111.bgr).r;
// 		vec4 sc000 = texture1D(texTransfunc, c000);
// 		vec4 sc001 = texture1D(texTransfunc, c001);
// 		vec4 sc010 = texture1D(texTransfunc, c010);
// 		vec4 sc011 = texture1D(texTransfunc, c011);
// 		vec4 sc100 = texture1D(texTransfunc, c100);
// 		vec4 sc101 = texture1D(texTransfunc, c101);
// 		vec4 sc110 = texture1D(texTransfunc, c110);
// 		vec4 sc111 = texture1D(texTransfunc, c111);
// 		vec4 sampledColor = xyzd1.x * xyzd1.y * xyzd1.z * sc000
// 						  + xyzd1.x * xyzd1.y * xyzd0.z * sc001
// 						  + xyzd1.x * xyzd0.y * xyzd1.z * sc010
// 						  + xyzd1.x * xyzd0.y * xyzd0.z * sc011
// 						  + xyzd0.x * xyzd1.y * xyzd1.z * sc100
// 						  + xyzd0.x * xyzd1.y * xyzd0.z * sc101
// 						  + xyzd0.x * xyzd0.y * xyzd1.z * sc110
// 						  + xyzd0.x * xyzd0.y * xyzd0.z * sc111;
		///////////////////////////////////////////////////

        color = color + sampledColor * vec4(sampledColor.aaa, 1.0) * (1.0 - color.a);
        if(color.a > 0.99)
            break;
    }
    
    color = color + vec4(bg.rgb, 0.0) * (1.0 - color.a);
    color.a = 1.0;
    gl_FragColor = color;
}