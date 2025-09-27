#include <stdio.h>

int main() {
int N;
scanf("%d", &N);

int arr[N];
long long total = 0;
int max = 0;

for (int i = 0; i < N; i++) {
    scanf("%d", &arr[i]);
    total += arr[i];
    if (arr[i] > max) {
        max = arr[i];
    }
}

int count = 0;
int days[N];
for (int i = 0; i < N; i++) {
    if (arr[i] == max) {
        days[count] = i + 1; 
        count++;
    }
}

double percentage = ((double)(max * count) / total) * 100.0;

printf("Max : %d\n", max);
printf("Count : %d\n", count);
printf("days :");
for (int i = 0; i < count; i++) {
    printf(" %d", days[i]);
}
printf("\n");
printf("Percentage : %.3f%%\n", percentage);

return 0;

}