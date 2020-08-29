#include <merlin-operational-research/merlin_a_star.hpp>

namespace merl
{
    AStar::AStar(MODE mode) : mode_(mode)
    {}
    std::list<AStar::State *> AStar::launch(AStar::State * init)
    {
        using State = AStar::State;
        std::list<State *> path;
        std::unordered_map<State *, State *> parents; // <State, Parent state>
        std::unordered_map<State *, double> g;        // <State, Cost/quality criterion value taken to reach this state>
        std::unordered_map<State *, double> f;        // <State, Optimistic estimation of the cost/quality criterion value taken to find the solution passing through this state>
        std::list<State *> AF;                        // List of States to process
        std::list<State *> DV;                        // List of already processed states
        State * current = nullptr;
        std::list<State*> neighbours;

        AF.push_back(init);
        g[init] = 0;
        f[init] = 0;
        bool found(false);

        while(!found && !AF.empty())
        {
            current = AF.front();
            AF.erase(std::begin(AF));
            DV.push_back(current);

            if(current->isSolution())
                found = true;
            else
            {
                neighbours = current->successors();
                for(auto & n : neighbours)
                {
                    if(!contains(AF, n) && !contains(DV, n))
                    {
                        g[n] = g[current] + current->k(n);
                        f[n] = g[n] + n->h();
                        parents[n] = current;
                        insert_ordered(AF, n, f);
                    }
                    else if((mode_==MODE::MAXIMIZE && g[n] < g[current]+current->k(n)) || (mode_==MODE::MINIMIZE && g[n] > g[current]+current->k(n)))
                    {
                        auto safe_comp = [&n](State * s){
                            bool res = s->equals(n);
                            if(res)
                                delete s;

                            return res;
                        };
                        AF.erase(std::remove_if(std::begin(AF), std::end(AF), safe_comp), std::end(AF));
                        DV.erase(std::remove_if(std::begin(DV), std::end(DV), safe_comp), std::end(DV));

                        g[n] = g[current] + current->k(n);
                        f[n] = g[n] + n->h();
                        parents[n] = current;
                        insert_ordered(AF, n, f);
                    }
                    else // Already existing node
                        delete n;
                }
            }
        }

        if(found)
        {
            path.push_back(current);
            while(!current->equals(init))
            {
                current = parents[current];
                path.insert(std::begin(path), current);
            }
        }

        //Release memory for the states not belonging to the solution path
        for(auto & s : AF)
            delete s;

        for(auto & s : DV)
        {
            if(!contains(path, s))
                delete s;
        }

        return path;
    }
    void AStar::release_path(std::list<State *> & path)
    {
        for(auto & s : path)
            delete s;

        path.clear();
    }
    void AStar::insert_ordered(std::list<AStar::State *> & vec, AStar::State * s, const std::unordered_map<AStar::State *, double> & f) const
    {
        bool inserted(false);
        for(std::list<AStar::State*>::const_iterator cit = vec.cbegin(); !inserted && cit != vec.cend(); ++cit)
        {
            bool max_cond = (mode_ == MODE::MAXIMIZE && f.at(s) > f.at(*cit));
            bool min_cond = (mode_ == MODE::MINIMIZE && f.at(s) < f.at(*cit));
            if(max_cond || min_cond)
            {
                vec.insert(cit, s);
                inserted = true;
            }
        }
        if(!inserted)
            vec.push_back(s);
    }
}
