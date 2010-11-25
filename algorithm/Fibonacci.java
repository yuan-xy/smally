package algorithm;
import java.lang.reflect.Method;

/*
 * 
 * Created on 2005-11-5
 */
/**
 * @author ylt
 */
public class Fibonacci {
	/**
	 * 时间复杂度O(指数级)
	 * @param n 
	 * @return
	 */
	public static long fib1(int n){
		if(n==0||n==1) return 1;
		return fib1(n-1)+fib1(n-2);
	}
	
	/**
	 * 时间复杂度O(n)
	 * @param n 
	 * @return
	 */
	public static long fib2(int n){
		if(n==0||n==1) return 1;
		long n_2=1;
		long n_1=1;
		long ret=0L;
		for(int i=2;i<=n;i++){
			ret=n_2+n_1;
			n_2=n_1;
			n_1=ret;
		}
		return ret;
	}
	
	/**
	 * 时间复杂度O(n)
	 * @param n 
	 * @return
	 */
	public static long fib3(int n){
		if(n==0||n==1) return 1;
		if(n%2==1){
			return fib3_0(n-1,true);//?对奇数如何处理
		}else{
			return fib3_0(n,false);
		}
	}
	private static long fib3_0(int n,boolean isPrime){
		long b=1;
		long a=1;
		long tmp=0L;
		for(int i=1;i<=n/2;i++){
			tmp=b+2*a;
			b=b+a;
			a=tmp;
		}
		if(isPrime) return a;
		return b;
	}
	
	/**
	 * 时间复杂度O(lgn)
	 * 对于变换a=t1*a+t2*b;
	 *         b=t3*a+t4*b;
	 * Fibonacci的变换为
	 * 			a=a+b
	 * 			b=a
	 * 所以初始值为t1=1,t2=1,t3=1,t4=0;
	 * fib(0)=1,fib(1)=1,所以a=1,b=1
	 * @param n 
	 * @return
	 */
	public static long fib4(int n){
		if(n==0||n==1) return 1;
		long t1=1,t2=1,t3=1,t4=0;
		long a=1,b=1;
		while(true){
			if(n%2==1){
				long tmp=t1*a+t2*b;
				b=t3*a+t4*b;
				a=tmp;
			}
			long tt1=t1*t1+t2*t3;
			long tt2=t1*t2+t2*t4;
			long tt3=t3*t1+t4*t3;
			long tt4=t2*t3+t4*t4;
			t1=tt1;
			t2=tt2;
			t3=tt3;
			t4=tt4;
			if((n=n/2)<2) break;
		}
		return t3*a+t4*b;
	}

	
	
	public static void main(String[] args){
		test("fib1",35);
		test("fib2",40);
		test("fib2",40000000);
		test("fib3",40000000);
		test("fib4",40000000);
		
	}
	public static void test(String f,int n) {
		try {
			Method method=Fibonacci.class.getMethod(f,new Class[]{int.class});
			long t1=0L;
			long t2=0L;
			t1=System.currentTimeMillis();
			Object obj=method.invoke(null,new Object[]{new Integer(n)});
			t2=System.currentTimeMillis();
			System.out.println(obj);
			System.out.println(f+"耗时："+(t2-t1));
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	
	
	
}
