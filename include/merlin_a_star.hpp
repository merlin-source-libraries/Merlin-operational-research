#ifndef A_STAR_H
#define A_STAR_H

#include <algorithm>
#include <list>
#include <unordered_map>

namespace merl
{
    class AStar final
    {
        public:
            class State
            {
                public:
                    virtual ~State() = default;
                    virtual bool isSolution() const = 0;
                    virtual std::list<State *> successors() const = 0;
                    virtual double h() const = 0;
                    virtual double k(const State * s) const = 0;
                    virtual bool equals(const State * s) const = 0;
            };

            enum class MODE {MINIMIZE, MAXIMIZE};

            AStar(MODE mode);
            std::list<State *> launch(State * init);

        private:
            MODE mode_;

            template <template <typename...> class Container>
            bool contains(const Container<State *> & vec, State * s) const
            {
                return std::find_if(std::begin(vec), std::end(vec), [&s](State * st){return st->equals(s);}) != std::end(vec);
            }
            void insert_ordered(std::list<State *> & vec, State * s, const std::unordered_map<State *, double> & f) const;
    };
}

#endif // A_STAR_H
