/*
 * 
 * Created on 2005-11-5
 */
package algorithm;

/**
 * @author ylt
 */
public class Factorial {
	
	public long fac1(int n){
		if(n==0) return 1;
		return n*fac1(n-1);
	}
	
	public long fac2(int n){
		if(n==0) return 1;
		long ret=0;
		for(int i=1;i<n;i++){
			ret=i*ret;
		}
		return ret;
	}
	
	public long fac3(int n){
		if(n==0) return 1;
		return fac3_0(1,n,1);
	}
	private long fac3_0(int initial,int end,long ret){
		if(initial>end) return ret;
		return fac3_0(initial+1,end,ret*initial);
	}
	
}
