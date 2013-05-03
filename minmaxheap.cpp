//Alex Daniels
//last fix:
//fixed a lot of run time issues
//old deletion operation would result in at minimum
//quadratic time due to a copy of the container
//for each deletion.
//commented important parts
//maxHeapify is taken from the textbook
//minheapify is just a modified maxheapify
//extract min and max are similar
//vector containers are public members
//primarily to avoid dealing with getters and setters

#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <fstream>

using namespace std;

typedef struct
{
	int key;
	int index;
}HeapKey;


class MinMax
{
	public:
		int parent(int i) { return (floor(i - 1) / 2); }
		int leftchild(int i) { return (2 * i + 1); }
		int rightchild(int i) { return (2 * i + 2); }
		HeapKey getMax() { return minHeap[0]; }
		HeapKey getMin() { return minHeap[0]; }
		void maxHeapify(int i);
		void minHeapify(int i);
		void heapsort();
		HeapKey extractMax();
		HeapKey extractMin();
		void insert(HeapKey k);
		vector<HeapKey> minHeap;
		vector<HeapKey> maxHeap;
};


int main(int argc, char** argv)
{
	if(argc == 1)
		cout << "No arguments" <<endl;

	else
	{	
		int n;
		int count = 0, count2 = 0;
		MinMax MM;			//MinMax
		HeapKey hk;	
		//in case of needed display of max and min 
		//after extraction
		//HeapKey extractmin, extractmax;
		ifstream file;
		//clock_t c1, c2;

		//c1 = clock();
		file.open(argv[1]);
		if(file.fail())
		{
			cerr << "Error opening " << argv[1] <<endl;
			exit(1);
		}
		else
			cout << "File Opened Successfully" <<endl << endl;
	
		while(count++ <= 524288)
		{
			file >> n;
			hk.key = n;
			MM.insert(hk);
		}
		//remove 1/2 of elements read in
		while(count2++ <= 131072)
		{
			MM.extractMax();
			MM.extractMin();
		}
		//read in the rest
		while(count++ <= 1048576)
		{
			file >> n;
			hk.key = n;
			MM.insert(hk);
		}
		//remove 1/2 of total left
		while(count2++ <= 262144)
		{	
			MM.extractMax();
			MM.extractMin();
		}

		/*c2 = clock();
		double diff = difftime(c2, c1);
		cout << "Done in: " << diff/CLOCKS_PER_SEC << " seconds" <<endl;
		*/
		cout << "Done" <<endl;
	
	}	
}

void MinMax::maxHeapify(int i)
{
	int l = leftchild(i);
	int r = rightchild(i);
	int largest;
	int indexA, indexB;

	if(l < maxHeap.size() && maxHeap[l].key > maxHeap[i].key)
		largest = l;
	else
		largest = i;

	if(r < maxHeap.size() && maxHeap[r].key > maxHeap[i].key)
		largest = r;

	if(largest != i)
	{
		indexA = maxHeap[i].index;
		indexB = maxHeap[largest].index;
		//maxheap[i] == struct heapkey with indexA as second component
		iter_swap(maxHeap.begin() + i, maxHeap.begin() + largest);
		//do the swap. iter_swap required for non-primative types
		maxHeap[indexA].index = largest;
		maxHeap[indexB].index= i;
	
		if(i > 0)	
			maxHeapify(parent(i));
	}
}

void MinMax::minHeapify(int i)
{
	int left = leftchild(i);
	int right = rightchild(i);
	int smallest;
	int indexA,indexB;			
	int heapsize = minHeap.size();

	if(left < heapsize && minHeap[left].key < minHeap[i].key)
		smallest = left;
	else
		smallest = i;

	if(right < heapsize && minHeap[right].key < minHeap[smallest].key)
		smallest = right;
	if(smallest != i)
	{
		indexA = minHeap[i].index;
		indexB = minHeap[smallest].index;
		//minheap[i] == struct heapkey with indexA as second component
		iter_swap(minHeap.begin() + i, minHeap.begin() + smallest);
		
		//maxHeap[indexA].index = smallest;
		maxHeap[indexB].index = i;
	
		if(i > 0)
			minHeapify(parent(i));
	}	
}

void MinMax::insert(HeapKey k)
{
	//insert into minheap then into maxheap
	minHeap.push_back(k);
	maxHeap.push_back(k);
	
	//in case of error, this 'checks' and makes sure 
	//there is no segementation fault if one heap grows faster
	//than the other
	int heapsize1 = minHeap.size();
	int heapsize2 = maxHeap.size();

	minHeap[heapsize1 - 1].index = (heapsize1 - 1);
	maxHeap[heapsize2 - 1].index = (heapsize2 - 1);

	minHeapify(parent(heapsize1 - 1));
	maxHeapify(parent(heapsize2 -1));

}


HeapKey MinMax::extractMax()
{
	HeapKey max, min;	
	int heapsize = maxHeap.size();

	if(heapsize < 1)
		cerr << "maxHeap underflow" <<endl;	//throw an error if no heap
	
	max = maxHeap[0];							//house keeping to return this value
	maxHeap[0] = maxHeap[heapsize - 1];			//maxheap beginning  =  maxheap tail
	minHeap[max.index] = minHeap[heapsize -1];	//fixing minheap to removed indexed element
	
	//pop_back() replaces erase()
	//the former runs O(1), whereas 
	//in this situation erase() would run closer to O(n^2)
	//pop_back() removes the maxHeap[heapsize - 1] element
	//without copying the vector each time it's called.
	//so for each successive extractMax() call the vector's 
	//contents should stay in the same location
	//obviously not the best, but the resize will be called upon 
	//insertion otherwise a resize() call would be necessary at the end
	//to avoid unallocated memory
	//and the tails severed.

	//So stays O(2lgn) = O(lgn) for due to the heapifying process.
	//instead of omega(n^2 lgn) for a regular erase() call.
	
	maxHeap.pop_back();						//pops and removes object without a resize
	minHeap.pop_back();						//pops and removes object without a resize

//	cout << "max: " <<heapsize1 <<endl;
	
	//Now fix the heaps
	maxHeapify(heapsize - 1);
	minHeapify(heapsize - 1);
	return max;								//for displaying purposes
}


HeapKey MinMax::extractMin()
{
	HeapKey min;
	int heapsize = minHeap.size();
	
	if(heapsize < 1)
		cerr << "minHeap Underflow" <<endl;		//throw an error if no heap
	
	min = minHeap[0];							//working on housekeeping
	minHeap[0] = minHeap[heapsize - 1];			//copying tail value to min
	maxHeap[min.index] = minHeap[heapsize - 1];	//adjusting accompaning maxheap
	
	//pop_back() replaces erase()
	//the former runs O(1), whereas 
	//in this situation erase() would run closer to O(n^2)
	//pop_back() removes the maxHeap[heapsize - 1] element
	//without copying the vector each time it's called.
	//so for each successive extractMin() call the vector's 
	//contents should stay in the same location
	//and the tails severed.
	//So stays O(2lgn) = O(lgn) for due to the heapifying process.

	minHeap.pop_back();						//Pops and removes object without a resize
	maxHeap.pop_back();						//pops and removes object without a resize
	
	//fixed the heaps
	minHeapify(heapsize - 1);
	maxHeapify(heapsize - 1);		
	return min;								//for displaying purposes if need be
}
