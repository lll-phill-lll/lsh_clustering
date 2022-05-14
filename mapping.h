class Dict {
public:
    enum Letter {
        A,
        C,
        G,
        T,
        EXTRA
    };


    int operator[](char c) const {
        switch (c) {
            case 'A': return A;
            case 'C': return C;
            case 'G': return G;
            case 'T': return T;

            default: return -1;

        }
    }


};
