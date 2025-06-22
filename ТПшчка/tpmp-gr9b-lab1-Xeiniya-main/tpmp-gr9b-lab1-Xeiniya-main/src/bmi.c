#include <stdio.h>

int main() 
{
float height=0, weight=0;

printf("Enter your height(cm): ");
scanf("%f",&height);

printf("Enter your weight(kg): ");
scanf("%f", &weight);

if(weight > (height - 100)) { #formula check
printf("It will be good for you to lose weight\n");
}
else {
printf("Your weight is normal\n");
}

return 0;
}
