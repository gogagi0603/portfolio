#include <bits/stdc++.h>
using namespace std;

int n, m, t, k;
int dap, dap_x, dap_y;
struct abc {
    int x, y;
} a[111];

void go(int x, int y) {
    int cnt = 0;
    for (int i = 1; i <= t; i++) {
        if (x <= a[i].x && a[i].x <= x+k &&
            y <= a[i].y && a[i].y <= y+k) cnt++;
    }
    if (cnt > dap) {
        dap_x = (x+y)/2;
        dap_y = (x-y)/2;
        dap = cnt;
    }
}

int main() {
    cin >> n >> m >> t >> k;
    for (int i = 1; i <= t; i++) {
        int x, y;
        cin >> x >> y;
        a[i].x = x+y, a[i].y = x-y;
    }

    dap = dap_x = dap_y = -1;
    for (int i = 1; i <= t; i++) {
        for (int j = 1; j <= t; j++) {
            int x = a[i].x, y = a[j].y;
            if ((x+y)%2) go(x+1, y), go(x-1, y), go(x, y+1), go(x, y-1);
            else go(x, y);
        }
    }

    cout << dap_x+k/2 << " " << dap_y << "\n" << dap;

    return 0;
}
