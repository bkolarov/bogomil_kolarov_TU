#include <math.h>
#include <stdio.h>

int main() {
	double a, b, c;
	double x, x1, x2;
	double re, d, im;

	printf("enter a, b ,c\n");
	scanf("%lf%lf%lf", &a, &b, &c);

	if (a == 0 && b == 0) {
		printf("degenerate equation\n");
	} else {
		if (a == 0) {
			x = -(c / b);
			printf("linear equation \n x = %lf\n", x);
		} else {
			if (c == 0) {
				x1 = - (b / a);
				x2 = 0;

				printf("two roots:\n x1 = %lf\n x2 = %lf\n", x1, x2);
			} else {
				re = - (b / (2*a));
				d = pow(b,2) - (4*a*c);
				im = sqrt(fabs(d)) / (2*a);
				if (d > 0) {
					x1 = re + im;
					x2 = re - im;
					
					printf("two roots:\n x1 = %lf\n x2 = %lf\n", x1, x2);
				} else {
					if (d < 0) {
						printf("(%lf,%lf)\n(%lf,%lf)\ncomplex roots\n", re, im, re, -im);
					} else {
						x1 = re;
						x2 = x1;

						printf("two equal roots:\n %lf = %lf\n", x1, x2);
					}
				}
			}
		}

	}

	return 0;
}
