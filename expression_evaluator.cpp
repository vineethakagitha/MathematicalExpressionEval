#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
int* evaluate(char*);
int mul_op(int, FILE*);
int div_op(int, FILE*);
int add_op(int, FILE*);
int sub_op(int, FILE*);
int pow(int, FILE*);
void main()
{
	char filename[10];
	int *res,i=0,ch;
	FILE *fd,*fp;
	printf("Enter Input filename: ");
	scanf("%s",filename);
    res=evaluate(filename);
	for (i = 0,printf("\n\n\n"); i < 5; i++)
		printf("%d\t%d\n", res + i, *(res + i));
	fd = fopen("outputfile.txt","w");
	fp = fopen(filename, "r");
	i = 0;
	while (!feof(fp))
	{
		while (!feof(fp) && (ch = fgetc(fp)) != '\n')
		{
			fprintf(fd, "%c", ch);
		}
		fprintf(fd, "=%d\n", res[i++]);
	}
}
int* evaluate(char *input)
{
	FILE *fp;
	int  d1;
	char  temp='\n';
	int  *res,idx = 0,i=0,result;
	fp = fopen(input, "r");
	res = (int*)malloc(sizeof(int) * 20);
	idx = 0;
	while (!feof(fp) && fscanf(fp, "%d", &d1))
	{
		if (fscanf(fp, "%c", &temp) && temp != '\n')
		{
			switch (temp)
			{
			case '*':res[idx++] = mul_op(d1, fp);
				     break;
			case '/':res[idx++] = div_op(d1, fp);
				break;
			case '-': res[idx++] = sub_op(d1, fp);
				break;
			case '+': res[idx++] = add_op(d1, fp);
				break;
			case '\0': for (i = 0; i < idx; i++)
				printf("%d\t%d\n", *res + i, res + i);
				fclose(fp);
				return res;
				break;
			}
		}
		else{
			res[idx++] = d1;
		}
	}
	fclose(fp);
		return res;
}
int prec(char op1, char op2)
{
	switch (op1)
	{
	case '/':return 0;
		break;
	case '*': if (op2 == '/')
		return 1;
			  else return 0;
			  break;
	case '+': if (op2 == '+' || op2 == '-')
		return 0;
			  else return 1;
			  break;
	case '-': if (op2 == '-')
		return 0;
			  else return 1;
			  break;
	default:return 0;
		break;
	}
	
}
int div_op(int d1,FILE *fp)
{
	int d2, r=1,op;
	char temp='\n';
	fscanf(fp, "%d", &d2);
	if (fscanf(fp, "%c", &temp) && temp != '\n')
	{
		op = prec('/', temp);
		if (temp == '*' && (temp = fgetc(fp)) && temp == '*')
			{
				d2 = pow(d2, fp);
				r = d1 / d2;
				return r;
			}
		else if (temp == '*')
				ungetc(temp, fp);
			r = d1 / d2;
			switch (temp)
			{
			case '*':
				r = mul_op(r, fp);
				break;
			case '/':r = div_op(r, fp);
				break;
			case '-': r = sub_op(r, fp);
				break;
			case '+': r = add_op(r, fp);
				break;
			}
		
	}
	else
		r = d1/d2;
	return r;

}
int sub_op(int d1, FILE *fp)
{
	int d2, r=0, op;
	char temp;
	fscanf(fp, "%d", &d2);
	if (fscanf(fp, "%c", &temp) && temp != '\n')
	{
		op = prec('-', temp);
		if (op)
		{
			switch (temp)
			{
			case '*': r = mul_op(d2, fp);
				break;
			case '/':r = div_op(d2, fp);
				break;
			case '-': r = sub_op(d2, fp);
				break;
			case '+': r = add_op(d2, fp);
				break;
			}
			r = d1 - r;
		}
		else
		{
			r = d1 - d2;
			switch (temp)
			{
			case '*': r = mul_op(r, fp);
				break;
			case '/':r = div_op(r, fp);
				break;
			case '-': r = sub_op(r, fp);
				break;
			case '+': r = add_op(r, fp);
				break;
			}
		}
	}
	else
		r = d1 - d2;
	return r;

}
int add_op(int d1, FILE *fp)
{
	int d2, r=0, op;
	char temp;
	fscanf(fp, "%d", &d2);
	if (fscanf(fp, "%c", &temp) && temp != '\n')
	{
		op = prec('+', temp);
		if (op)
		{
			switch (temp)
			{
			case '*': r = mul_op(d2, fp);
				break;
			case '/':r = div_op(d2, fp);
				break;
			case '-': r = sub_op(d2, fp);
				break;
			case '+': r = add_op(d2, fp);
				break;
			}
			r = d1 + r;
		}
		else
		{
			r = d1 + d2;
			switch (temp)
			{
			case '*': r = mul_op(r, fp);
				break;
			case '/':r = div_op(r, fp);
				break;
			case '-': r = sub_op(r, fp);
				break;
			case '+': r = add_op(r, fp);
				break;
			}
		}
	}
	else
		r = d1 + d2;
	return r;

}
int pow(int d1, FILE *fp)
{
	int d2, r=1,i;
	char temp;
	fscanf(fp, "%d", &d2);
	for (i = d2; i != 0; i--)
		r = r*d1;
	if (fscanf(fp,"%c",&temp) && temp!='\n')
	{
		switch (temp)
		{
		case '*': r = mul_op(r, fp);
			break;
		case '/':r = div_op(r, fp);
			break;
		case '-': r = sub_op(r, fp);
			break;
		case '+': r = add_op(r, fp);
			break;
		}
	}
	return r;
}
int mul_op(int d1, FILE *fp)
{
	int d2, r = 1, op,tp;
	char temp;
	if ((tp = fgetc(fp)) && (char)tp == '*')
	{
		r = pow(d1, fp);
		return r;
	}
	ungetc(tp, fp);
	fscanf(fp, "%d", &d2);
	if (fscanf(fp, "%c", &temp) && temp != '\n')
	{
		op = prec('*', temp);
		if (op)
		{
			switch (temp)
			{
			case '*': r = mul_op(d2, fp);
				break;
			case '/':r = div_op(d2, fp);
				break;
			case '-': r = sub_op(d2, fp);
				break;
			case '+': r = add_op(d2, fp);
				break;
			}
			r = d1*r;
		}
		else
		{
			r = d1*d2;
			switch (temp)
			{
			case '*': r = mul_op(r, fp);
				break;
			case '/':r = div_op(r, fp);
				break;
			case '-': r = sub_op(r, fp);
				break;
			case '+': r = add_op(r, fp);
				break;
			}
		}
	}
	else
		r = d1*d2;
	return r;

}
