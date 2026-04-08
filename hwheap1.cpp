#include <iostream>
#include <vector>
using namespace std;


void heap_shift_up(vector<int> &heap, int i)
{
    while (i > 0)
    {
        int p = (i - 1) / 2;
        if (heap[i] >= heap[p])
            return;

        swap(heap[i], heap[p]);
        i = p;
    }
}


void heap_shift_dwn(vector<int> &heap, int i)
{
    int n = heap.size();
    while (true)
    {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int min = i;

        if (l < n && heap[l] < heap[min])
            min = l;

        if (r < n && heap[r] < heap[min])
            min = r;

        if (min != i)
        {
            swap(heap[i], heap[min]);
            i = min;
        }
        else
            break;
    }
}


void push_heap(vector<int> &heap, int x)
{
    heap.push_back(x);
    heap_shift_up(heap, heap.size() - 1);
}


void pop_heap(vector<int> &heap)
{
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty())
        heap_shift_dwn(heap, 0);
}


void make_heap(vector<int> &heap)
{
    for (int i = heap.size() / 2 - 1; i >= 0; --i)
        heap_shift_dwn(heap, i);
}

int main()
{
    int n;
    cin >> n;

    vector<int> heap;

    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        push_heap(heap, x);
    }

    int result = 0;

    while (heap.size() > 1)
    {
        int a = heap[0];
        pop_heap(heap);

        int b = heap[0];
        pop_heap(heap);

        int s = a + b;
        result += s;

        push_heap(heap, s);
    }

    cout << result;
}