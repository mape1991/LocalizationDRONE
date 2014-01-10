#define N 256

void initFIR (int *signal)
{
	for (int i=0;i<N;i++)
		signal[i] = 0;
}

void filtreFIR1 (int *coeff, int *signal)
{
	int result=0;
	for (int i=0;i<N;i++) 
		result = result + coeff[i]*signal[i]
}