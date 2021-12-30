__kernel void jarowinkler(__global char *a, __global char *b,
                          __global int *as, __global int *bs,
                          __local int *aboo, __local int *bboo,
                          __global float *res, const int len1,const int len2) {
	const int idx = get_global_id(0);
  const int jdx = get_global_id(1);

	if (idx < len1 && jdx < len2){
    if (as[idx]!=0 && bs[jdx]!= 0){
      //printf("Test print from ker");
      char *astr = &a[idx];
      char *bstr = &b[jdx];

      for(int it=0 ; it < bs[jdx] ; it++){
        bboo[it] = 0;
      }
      for(int it=0 ; it < bs[jdx] ; it++){
        aboo[it] = 0;
      }

      int match_distance = max(as[idx],bs[jdx])/2-1;
      int matches = 0;
      for(int it1=0; it1 < as[idx] ; it1++){
        int sta = max(it1-match_distance,0);
        int end = min(it1+match_distance+1,bs[jdx]);
        for(int it2=sta ; it2 < end ; it2++){
          if(!bboo[it2] && astr[it1] == bstr[it2]){
            aboo[it1] = true;
            bboo[it2] = true;
            matches++;
            break;
          }
        }
      }
      if(matches != 0){
        float t = 0.0;
        int it2 = 0;
        for(int it1 = 0; it1 < as[idx]; it1++){
          if(aboo[it1]){
            while(!bboo[it2]) it2++;
            if(astr[it1]!=bstr[it2]) t+=0.5;
            it2++;
          }
        }
        float m = matches;
        float v = (m / as[idx] + m / bs[jdx] + (m - t) / m) / 3.0;

        if(v>0.7){
          int common_prefix = 0;
          int l = min(min(as[idx],bs[jdx]),4);
          for (; common_prefix < l; ++common_prefix){
            if(astr[common_prefix]!=bstr[common_prefix]) break;
          }
          v = v + 0.1 * common_prefix * (1-v);
        }
        res[idx+len2*jdx] = v;
      }
    }
  }
}
