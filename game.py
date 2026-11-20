# пятнашки, одно состояне (таблицу) можно в int64 (даже 32)
# индекс - tuple, просто перечисление таблицы

from typing import Callable
from collections import deque

Graph = tuple[int, int, int, int, int, int, int, int, int]


def bfs_way(g: Callable[[Graph], list[Graph]], v0: Graph, v1: Graph) -> list[Graph]:
    parent: dict[Graph, Graph | None] = {}
    q = deque()
    parent[v0] = None
    q.append(v0)

    while q:
        v = q.popleft()
        if v == v1:
            way = []
            while v is not None:
                way.append(v)
                v = parent[v]
            way.reverse()
            return way

        for u in g(v):
            if u not in parent:
                parent[u] = v
                q.append(u)
    return []  # путь не найден


v0 = (1, 3, 2, 4, 7, 6, 5, 8, 0)

v1 = (1, 2, 3, 4, 5, 6, 7, 8, 0)


def next(v: Graph) -> list[Graph]:
    index = v.index(0)
    zi = index // 3
    zj = index % 3

    if zi == 0 and zj == 0:
        return [move(v, 3), move(v, 1)]

    if zi == 0 and zj == 1:
        return [move(v, 4), move(v, 0), move(v, 2)]

    if zi == 0 and zj == 2:
        return [move(v, 5), move(v, 1)]

    if zi == 1 and zj == 0:
        return [move(v, 0), move(v, 6), move(v, 4)]

    if zi == 1 and zj == 1:
        return [move(v, 1), move(v, 7), move(v, 3), move(v, 5)]

    if zi == 1 and zj == 2:
        return [move(v, 2), move(v, 8), move(v, 4)]

    if zi == 2 and zj == 0:
        return [move(v, 3), move(v, 7)]

    if zi == 2 and zj == 1:
        return [move(v, 4), move(v, 6), move(v, 8)]

    if zi == 2 and zj == 2:
        return [move(v, 5), move(v, 7)]


# k = 3i+j
def move(v: Graph, k: int) -> Graph:
    z = v.index(0)
    u = list(v)
    u[z], u[k] = u[k], u[z]
    return tuple(u)


way: list[Graph] = bfs_way(next, v0, v1)

print(way)
