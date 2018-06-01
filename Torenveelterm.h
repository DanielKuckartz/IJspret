#ifndef TORENVEELTERM_H
#define TORENVEELTERM_H

#include <fstream>
#include <vector>

const int maxN = 18;

inline int teken (bool positief) { return (positief ? 1 : -1); }
inline int min (int a, int b) { return (a <= b ? a : b); }
inline int max (int a, int b) { return (a >= b ? a : b); }

struct voortbrenger {
	std::vector<long long> coefficienten; // representatie voortbrengende functie
	voortbrenger& operator*= (const voortbrenger& rhs);
	inline int size() const { return coefficienten.size(); }
	inline long long& operator[] (std::size_t index){return coefficienten[index];}
	inline const long long& operator[] (std::size_t index) const {
		return coefficienten[index];
	}
	voortbrenger& operator= (const voortbrenger& ander);
	voortbrenger& operator+= (const voortbrenger& rhs);
	friend voortbrenger operator+ (voortbrenger lhs, const voortbrenger& rhs);
};

struct veld {
	int x, y;
	veld& operator= (const veld& ander);
	inline bool operator == (const veld&  comp) const {
		return (x == comp.x && y == comp.y);
	}
	inline bool operator != (const veld& comp) const { return !(*this == comp); }
};

class Torenveelterm {
	private:
		int hoogte, breedte;
		bool bord[maxN][maxN]; // bord van verboden velden
		voortbrenger veelterm; // voortbrengende functie
	public:
		Torenveelterm(const char filename[]); // constructor
		void clear();
		void leesin(const char filename[]);
		Torenveelterm(bool rhsbord[maxN][maxN], int h, int b);
		Torenveelterm& operator=(const Torenveelterm& rhs);
		bool nulelement();
		veld vindkoning();
		veld vinddame(veld dame, veld koning);
		voortbrenger eigenschap1(veld dame);
		voortbrenger eigenschap2(veld dame, veld koning);
		voortbrenger berekenveelterm(veld dame, veld koning);
		voortbrenger berekenveelterm();
		long long nr_permutaties ();
};	

#endif
