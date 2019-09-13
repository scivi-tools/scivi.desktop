#ifndef TRIPLET_H
#define TRIPLET_H

namespace scivi {

namespace knowledge {

template <typename T1, typename T2, typename T3>
struct Triplet {
    T1 first;
    T2 second;
    T3 third;

    Triplet(T1 first, T2 second, T3 third) {
        this->first = first;
        this->second = second;
        this->third = third;
    }
};

class Concept;
using RelationTriplet = Triplet<Concept*, int, Concept*>;

}  // namespace knowledge
}  // namespace scivi

#endif  // TRIPLET_H
