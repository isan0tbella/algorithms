#include <iostream>
#include <vector>
using namespace std;

struct Node
{
    int val;
    int index;
};

bool comprare(const Node &a, const Node &b)
{
    return a.val < b.val;
}

void heap_shift_up(vector<Node> &heap, int i)
{
    while (i > 0)
    {
        int p = (i - 1) / 2;
        if (!(comprare(heap[p], heap[i])))
            return;

        swap(heap[p], heap[i]);
        i = p;
    }
}

void heap_shift_dwn(vector<Node> &heap, int i)
{
    while (true)
    {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int j = i;

        if (l < heap.size() && comprare(heap[j], heap[l]))
            j = l;
        if (r < heap.size() && comprare(heap[j], heap[r]))
            j = r;

        if (j != i)
        {
            swap(heap[i], heap[j]);
            i = j;
        }
        else
            break;
    }
}

void push_heap(vector<Node> &heap, Node v)
{
    heap.push_back(v);
    heap_shift_up(heap, heap.size() - 1);
}

void pop_heap(vector<Node> &heap)
{
    heap[0] = heap.back();
    heap.pop_back();
    heap_shift_dwn(heap, 0);
}

int main()
{
    int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];

    int k;
    cin >> k;

    vector<Node> heap;

    for (int i = 0; i < k; i++)
        push_heap(heap, {a[i], i});

    cout << heap[0].val << " ";

    for (int i = k; i < n; i++)
    {
        push_heap(heap, {a[i], i});

        while (!heap.empty() && heap[0].index <= i - k)
            pop_heap(heap);

        cout << heap[0].val;
    }
}