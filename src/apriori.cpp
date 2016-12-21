#include "stdafx.h"
#include "ItemSet.h"
using namespace std;
using namespace Github::Miginmrs::Apriori;

void print(ArrayList<ItemSet> itemSets) {
	for (ptr it = itemSets.begin(); it != itemSets.end(); ++it) {
		(*it)->print();
	}
}

int main(int nargs, const char *args[])
{
	if(nargs!=2) {
		cerr << "Usage : apriori filename";
		return 1;
	}
	string path = args[1];
	ifstream myfile(path);
	int support = 3000;
	myfile.unsetf(std::ios_base::skipws);
	unsigned line_count = std::count(
		istream_iterator<char>(myfile),
		istream_iterator<char>(),
		'\n');
	myfile = ifstream(path);
	myfile.unsetf(std::ios_base::skipws);
	ArrayList<ArrayList<int>> db(line_count);
	string line;
	int maxItem = 0;
	while (std::getline(myfile, line)) {
		int max = 1;
		for (auto& iter : line)
		{
			if (iter == ' ')
				max++;
		}

		ArrayList<int> items(max);
		istringstream iline(line);
		for (int i = 0; i < max; i++) {
			int item;
			iline >> item;
			if (maxItem < item)
				maxItem = item;
			items.push(move(item));
		}
		db.push(move(items));
	}
	int N = omp_get_max_threads();
	ArrayList<ItemSet> itemSets(0);
	{
		ArrayList<ArrayList<ItemSet>> itemSetsList(maxItem + 1);
		{
			ArrayList<ItemSet> defaultItemSets(maxItem + 1);
			for (int item = 0; item <= maxItem; item++) {
				ItemSet itemSet(item);
				defaultItemSets.push(move(itemSet));
			}
			for (int i = 0; i < defaultItemSets.len(); i++) {
				defaultItemSets[i].setoccurs(N);
			}
#pragma omp parallel for
			for (int i = 0; i < db.len(); i++) {
				for (int j = 0; j < defaultItemSets.len(); j++) {
					defaultItemSets[j].occur(db[i], omp_get_thread_num());
				}
			}
			ArrayList<ItemSet> frequentItemSets(maxItem + 1);
			for (int i = 0; i < defaultItemSets.len(); i++) {
				if (support < defaultItemSets[i].support()) {
					frequentItemSets.push(move(defaultItemSets.extract(i)));
				}
			}
			frequentItemSets.crop();
			itemSetsList.push(move(frequentItemSets));
		}
		while (true) {
			auto itemSets = itemSetsList[itemSetsList.len() - 1];
			ArrayList<ArrayList<ItemSet>> lists(itemSets.len());
#pragma omp parallel for
			for (int i = 0; i < itemSets.len(); i++) {
				ArrayList<ItemSet> list(itemSets.len());
				for (int j = i + 1; j < itemSets.len(); j++) {
					ItemSet itemSet = itemSets[i].plus(itemSets[j]);
					if (itemSet.isNull()) continue;
					list.push(move(itemSet));
				}
				list.crop();
				lists.push(move(list), i);
			}
			ArrayList<ItemSet>	computedItemSets = ArrayList<ItemSet>::concat(move(lists));
			for (int i = 0; i < computedItemSets.len(); i++) {
				computedItemSets[i].setoccurs(N);
			}
#pragma omp parallel for
			for (int i = 0; i < db.len(); i++) {
				for (int j = 0; j < computedItemSets.len(); j++) {
					computedItemSets[j].occur(db[i], omp_get_thread_num());
				}
			}
			ArrayList<ItemSet> frequentItemSets(computedItemSets.len());
			for (int i = 0; i < computedItemSets.len(); i++) {
				if (support < computedItemSets[i].support()) {
					frequentItemSets.push(move(computedItemSets.extract(i)));
				}
			}
			frequentItemSets.crop();
			if (!frequentItemSets.len())
				break;
			itemSetsList.push(move(frequentItemSets));
		}
		itemSetsList.crop();
		itemSets = itemSets.concat(move(itemSetsList));
	}
	print(itemSets);
}