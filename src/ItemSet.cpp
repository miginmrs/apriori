#include <iostream>
#include "ItemSet.h"
namespace Github { namespace Miginmrs { namespace Apriori {
	using namespace std;
	ItemSet::ItemSet(int k, int *items) : k(k), items(items), occsize(0), occurences(NULL) {
	}
	ItemSet::ItemSet(int x) :k(1), occsize(0), occurences(NULL)  {
		items = new int[1];
		items[0] = x;
	}
	ItemSet::ItemSet(ArrayList<int> const & list) :k(list.len()), occsize(0), occurences(NULL) {
		items = new int[k];
		for (int i = 0; i < k; i++) {
			items[i] = list[i];
		}
	}
	ItemSet::ItemSet(ItemSet &&itemSet) :k(itemSet.k), occsize(0), occurences(NULL) {
		items = itemSet.items;
		itemSet.items = NULL;
	}
	ItemSet::~ItemSet()
	{
		delete occurences;
		if (items != NULL)
			delete items;
	}
	
	bool ItemSet::isNull() const {
		return items == NULL;
	}
	void ItemSet::print() const {
		for (int i = 0; i < k; i++) {
			cout << items[i] << " ";
		}
		cout << endl;
	}
	ItemSet ItemSet::plus(ItemSet const& itemSet) const {
		ItemSet out = ItemSet(k + 1, new int[k + 1]);
		int diff = 0;
		int last = k - 1;
		for (int i = 0; i < last; i++) {
			if (items[i] != itemSet.items[i])
				return ItemSet(0, NULL);
			out.items[i] = items[i];
		}
		out.items[last] = items[last];
		out.items[k] = itemSet.items[last];
		return out;
	}

	void ItemSet::setoccurs(int size) {
		occsize = size;
		occurences = new int[size];
		for (int i = 0; i < size; i++)
			occurences[i] = 0;
	}

	bool ItemSet::occur(ArrayList<int> const & list, int i) const {
		int j = 0;
		for (int i = 0; i < k; i++) {
			if (j == list.len())
				return false;
			while (list[j++] != items[i])
				if (j == list.len())
					return false;
		}
		occurences[i]++;
		return true;
	}
	int ItemSet::support() const {
		int sum = 0;
		for (int i = 0; i < occsize; i++)
			sum += occurences[i];
		return sum;
	}
}}}