#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cout << "Enter the number of cows: ";
    cin >> N;

    vector<int> a(N);
    for (int i = 0; i < N; i++) cin >> a[i];

    sort(a.begin(), a.end(), greater<int>());

    int total = 0;

    for (int i = 0; i < N; i++) {
        int milk = a[i] - i;
        if (milk > 0)
            total += milk;
        else
            break;
    }

    cout << "The Result: " << endl;

    return 0;
}