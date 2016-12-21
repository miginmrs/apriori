namespace Github { namespace Miginmrs { namespace Apriori {
	template<class T>
	class ArrayList {
		int max, size;
		T **array;
		bool iscopy = false;
	public:
		ArrayList<T>(int max) : max(max), size(0) {
			array = new T*[max];
		}
		ArrayList<T>(ArrayList<T> &&a) : max(a.max), size(a.size) {
			array = a.array;
			a.array = NULL;
		}
		void operator= (ArrayList<T> &&a) {
			this->max = a.max;
			this->size = a.size;
			array = a.array;
			a.array = NULL;
		}
		ArrayList<T>(ArrayList<T> const &a) : max(a.max), size(a.size) {
			array = a.array;
			iscopy = true;
		}
		ArrayList<T>(T * &&array, int size) : max(size), size(size) {
			this->array = new T*[size];
			for (int i = 0; i < size; i++)
				this->array[i] = new T(move(array[i]));
		}
		~ArrayList<T>() {
			if (array == NULL || iscopy)
				return;
			for (int i = 0; i < size; i++)
				delete array[i];
			free(array);
		}
		T & operator[](int i) const {
			return *array[i];
		}
		bool push(T &&val) {
			if (size < max) {
				array[size++] = new T(move(val));
				return true;
			}
			return false;
		}
		bool push(T &&val, int index) {
			if (index < max) {
				array[index] = new T(move(val));
				if (size < index)
					size = index;
				return true;
			}
			return false;
		}
		bool push(T *&&val) {
			if (size < max) {
				array[size++] = val;
				return true;
			}
			return false;
		}
		int len() const {
			return size;
		}
		void crop() {
			array = (T**)realloc(array, sizeof(T)*size);
		}
		T * extract(int i) {
			T* p = array[i];
			array[i] = NULL;
			return p;
		}
		T *const * begin() const {
			return array;
		}
		T *const * end() const {
			return array + size;
		}
		static ArrayList<T> concat(ArrayList<ArrayList<T>> &&lists) {
			int size = 0, k = 0;
			for (int i = 0; i < lists.len(); i++) {
				size += lists[i].len();
			}
			ArrayList<T> out(size);
			for (int i = 0; i < lists.len(); i++) {
				for (int j = 0; j < lists[i].len(); j++) {
					out.array[out.size++] = lists[i].array[j];
					lists[i].array[j] = NULL;
				}
			}
			return out;
		}
	};
}}}