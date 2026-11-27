#include <iostream>
#include <vector>
#include <functional>
#include <queue>
#include <stdexcept>
#include <algorithm>

using namespace std;

class Vertex
{
public:
    virtual ~Vertex() = default;
    virtual int num_inputs() const = 0;
    virtual void set_input(int inp_idx, double inp_val) = 0;
    virtual int num_outputs() const = 0;
    virtual double get_output(int out_idx) = 0;
    virtual void calc_value() = 0;
};
struct Edge
{
    int out_vertex_id;
    int out_port_id;
    int inp_vertex_id;
    int inp_port_id;
};
class CalcGraph : public Vertex
{
    std::vector<Vertex *> vertex;
    std::vector<Edge> edges;
    std::vector<int> order;

public:
    int num_inputs() const override;
    void set_input(int inp_idx, double inp_val) override;
    int num_outputs() const override;
    double get_output(int out_idx) override;
    void calc_value() override;
    void set_data(const std::vector<Vertex *> &vertex, const std::vector<Edge>
                                                           &edges);
};

class PlusOperator : public Vertex
{
    double m_inp_val[2] = {0};
    double m_out_val = 0;

public:
    int num_inputs() const override
    {
        return 2;
    }
    void set_input(int inp_idx, double inp_val) override
    {
        if (inp_idx < 0 || inp_idx >= 2)
            throw out_of_range("Bad inp_idx in the PlusOperator::set_input");
        m_inp_val[inp_idx] = inp_val;
    }
    void calc_value() override
    {
        m_out_val = m_inp_val[0] + m_inp_val[1];
    }
    int num_outputs() const override
    {
        return 1;
    }
    double get_output(int out_idx) override
    {
        if (out_idx != 0)
            throw out_of_range("Bad out_idx");
        return m_out_val;
    }
};

std::vector<int> topo_sort(const std::vector<std::vector<int>> &g)
{
    std::vector<bool> visited(g.size());
    std::vector<int> result;

    std::function<void(int)> dfs = [&](int v)
    {
        visited[v] = true;
        for (int u : g[v])
            if (!visited[u])
                dfs(u);
        result.push_back(v);
    };

    for (int v = 0; v < g.size(); ++v)
        if (!visited[v])
            dfs(v);

    reverse(result.begin(), result.end());
    return result;
}

void CalcGraph::set_data(const std::vector<Vertex *> &vertex,
                         const std::vector<Edge> &edges)
{
    this->vertex = vertex;
    this->edges = edges;

    std::vector<std::vector<int>> g(this->vertex.size());

    for (Edge e : this->edges)
    {
        g[e.out_vertex_id].push_back(e.inp_vertex_id);
    }

    order = topo_sort(g);
}

int CalcGraph::num_inputs() const
{
    int count = 0;

    for (Vertex *v : vertex)
        count += v->num_inputs();

    count -= edges.size();

    return count;
}

void CalcGraph::set_input(int inp_idx, double inp_val)
{
    if (inp_idx < 0 || inp_idx >= num_inputs())
        throw out_of_range("Bad inp_idx");

    int current = 0;

    for (int v = 0; v < vertex.size(); ++v) // по вершинам
    {
        for (int p = 0; p < vertex[v]->num_inputs(); ++p) // по всем портам
        {
            bool connected = false;

            for (Edge e : edges)
            {
                if (e.inp_vertex_id == v && e.inp_port_id == p)
                    connected = true;
            }

            if (!connected)
            {
                if (current == inp_idx)
                {
                    vertex[v]->set_input(p, inp_val);
                    return;
                }

                ++current; // нашли свободный вход
            }
        }
    }
}

void CalcGraph::calc_value()
{
    for (int v : order)
    {
        for (Edge e : edges)
        {
            if (e.inp_vertex_id == v)
            {
                double value = vertex[e.out_vertex_id]->get_output(e.out_port_id);
                vertex[v]->set_input(e.inp_port_id, value);
            }
        }

        vertex[v]->calc_value();
    }
}

int CalcGraph::num_outputs() const
{
    int count = 0;

    for (int v = 0; v < vertex.size(); ++v)
    {
        for (int p = 0; p < vertex[v]->num_outputs(); ++p)
        {
            bool connected = false;

            for (Edge e : edges) // выход является началом ребра?
            {
                if (e.out_vertex_id == v && e.out_port_id == p)
                    connected = true;
            }

            if (!connected)
                ++count;
        }
    }

    return count;
}

double CalcGraph::get_output(int out_idx) // как в set_input, только выходы
{
    if (out_idx < 0 || out_idx >= num_outputs())
        throw out_of_range("Bad out_idx");

    int current = 0;

    for (int v = 0; v < vertex.size(); ++v)
    {
        for (int p = 0; p < vertex[v]->num_outputs(); ++p)
        {
            bool connected = false;

            for (Edge e : edges)
            {
                if (e.out_vertex_id == v && e.out_port_id == p)
                    connected = true;
            }

            if (!connected)
            {
                if (current == out_idx)
                    return vertex[v]->get_output(p);

                ++current;
            }
        }
    }

    throw out_of_range("Bad out_idx");
}

int main()
{
    PlusOperator p1, p2, p3;

    std::vector<Vertex *> vertex;
    std::vector<Edge> edges;

    vertex.push_back(&p1);
    vertex.push_back(&p2);
    vertex.push_back(&p3);

    edges.push_back(Edge{0, 0, 1, 0});
    edges.push_back(Edge{1, 0, 2, 0});

    CalcGraph cg;
    cg.set_data(vertex, edges);

    cout << cg.num_inputs() << endl;
    cout << cg.num_outputs() << endl;

    cg.set_input(0, 1);
    cg.set_input(1, 2);
    cg.set_input(2, 3);
    cg.set_input(3, 4);

    cg.calc_value();

    cout << cg.get_output(0) << endl;
}
