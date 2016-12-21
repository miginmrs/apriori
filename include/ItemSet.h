#include <list>
#include "ArrayList.h"
namespace Github { namespace Miginmrs { namespace Apriori {
	class ItemSet {
		int *items;
		int k;
		ItemSet(int k, int *items);
		int *occurences;
		int occsize;
	public:
		ItemSet(int x);
		ItemSet(ArrayList<int> const & list);
		ItemSet(ItemSet &&itemSet);
		~ItemSet();
		
		bool isNull() const;
		void print() const;
		ItemSet plus(ItemSet const& itemSet) const;
		void setoccurs(int size);
		bool occur(ArrayList<int> const & list, int i) const;
		int support() const;
	};
	
	typedef std::list<ItemSet>::const_iterator cptr;
	typedef const ItemSet *const *ptr;

	typedef ItemSet const ItemSetArray[];
	
}}}