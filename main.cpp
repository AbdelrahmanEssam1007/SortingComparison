#include <iostream>
#include <vector>
#include <random>
#include <chrono>

std::mt19937 gen(std::random_device{}());

int RandomNum(const int low, const int high) {
  std::uniform_int_distribution<int> dis(low, high);
  return dis(gen);
}

std::vector<int> GenerateVector(const uint32_t size) {
  std::vector<int> vec(size);
  for (int& elem : vec) {
    elem = RandomNum(-1e6, 1e6);
  }
  return vec;
}

// Bubble Sort: Swaps adjacent elements to sort the list.
void BubbleSort(std::vector<int>& vec) {
  const int sz = static_cast<int>(vec.size());
  for (int i = 0; i < sz; ++i) {
    bool swapped = false;
    for (int j = 0; j < sz - i - 1; ++j) {
      if (vec[j] > vec[j + 1]) {
        std::swap(vec[j], vec[j + 1]);
        swapped = true;
      }
    }
    if (!swapped) break; // if no swaps are made, break the loop.
  }
}

// Selection Sort: Finds the smallest element and swaps it to the front.
void SelectionSort(std::vector<int>& vec) {
  const int sz = static_cast<int>(vec.size());
  for (int i = 0; i < sz - 1; ++i) {
    int minIndex = i;
    for (int j = i + 1; j < sz; ++j) {
      if (vec[j] < vec[minIndex]) {
        minIndex = j; // find the smallest element.
      }
    }
    if (minIndex != i) {
      // only swap if the smallest element is not at the front.
      std::swap(vec[i], vec[minIndex]);
    }
  }
}

int RandomPartition(std::vector<int>& vec, const int low, const int high) {
  const int pivot = RandomNum(low, high); //select random pivot
  std::swap(vec[pivot], vec[high]); // swap pivot to high
  int i = low - 1;
  //comparison step
  for (int j = low; j < high; ++j) {
    if (vec[j] < vec[high]) {
      ++i;
      std::swap(vec[i], vec[j]);
    }
  }
  //insert pivot into place
  std::swap(vec[i + 1], vec[high]);
  return i + 1;
}

void QuickSort(std::vector<int>& vec, const int low, const int high) {
  if (low < high) {
    const int partitionIndex = RandomPartition(vec, low, high);
    QuickSort(vec, low, partitionIndex - 1);
    QuickSort(vec, partitionIndex + 1, high);
  }
}


void MaxHeapify(std::vector<int>& vec, const int n, const int i) {
  int largest = i; //parent
  const int left = 2 * i + 1; //left child
  const int right = 2 * i + 2; //right child

  if (left < n && vec[left] > vec[largest]) largest = left; // set left child to largest if parent is smaller
  if (right < n && vec[right] > vec[largest]) largest = right; // set right child to largest if parent is smaller
  if (largest != i) {
    std::swap(vec[i], vec[largest]); // swap parent with child if child is larger
    MaxHeapify(vec, n, largest);
  }
}

void HeapSort(std::vector<int>& vec) {
  const int sz = static_cast<int>(vec.size());
  //Build Max Heap;
  for (int i = sz / 2 - 1; i >= 0; i--) {
    MaxHeapify(vec, sz, i);
  }
  //heap sort
  for (int i = sz - 1; i > 0; --i) {
    std::swap(vec[0], vec[i]);
    MaxHeapify(vec, i, 0);
  }
}

void Merge(std::vector<int>& vec, int low, int mid, int high) {
  int nR = high - mid;
  int nL = mid - low + 1;
  std::vector<int> R(nR);
  std::vector<int> L(nL);
  for (int i = 0; i < nL - 1; i++) {
    L[i] = vec[low + i];
  }
  for (int i = 0; i < nR - 1; i++) {
    R[i] = vec[mid + i + 1];
  }
  int i = 0, j = 0;
  int k = low;
  while (i < nL && j < nR) {
    if (L[i] <= R[j]) {
      vec[k++] = L[i++];
    }
    else {
      vec[k++] = R[j++];
    }
  }
  while (i < nL) {
    vec[k++] = L[i++];
  }
  while (j < nR) {
    vec[k++] = R[j++];
  }
}

void MergeSort(std::vector<int>& vec, int low, int high) {
  if (low < high) {
    const int mid = (low + high) / 2;
    MergeSort(vec, low, mid);
    MergeSort(vec, mid + 1, high);
    Merge(vec, low, mid, high);
  }
}

void InsertionSort(std::vector<int>& vec, int low, int high) {
  for (int i = low + 1; i <= high; i++) {
    const int key = vec[i];
    int j = i - 1;
    while (j >= low && vec[j] > key) {
      vec[j + 1] = vec[j];
      j--;
    }
    vec[j + 1] = key;
  }
}

void HybridMerge(std::vector<int>& vec, int low, int high, int threshold) {
  if (high - low + 1 <= threshold) {
    InsertionSort(vec, low, high);
    return;
  }
  if (low < high) {
    const int mid = (low + high) / 2;
    HybridMerge(vec, low, mid, threshold);
    HybridMerge(vec, mid + 1, high, threshold);
    Merge(vec, low, mid, high);
  }
}


int KthSmallestElem(std::vector<int>& vec, const int low, const int high, const int k) {
  if (low == high) return vec[low];
  const int index = RandomPartition(vec, low, high);
  if (index == k) return vec[index]; // if equal return
  else if (index > k) return KthSmallestElem(vec, low, index - 1, k); //if smaller recur left sub array
  else return KthSmallestElem(vec, index + 1, high, k); //if larger recur right sub array
}

void PrintVec(std::vector<int>& vec) {
  for (const int elem : vec) {
    std::cout << elem << ' ';
  }
  std::cout << '\n';
}

// Measures the execution time of a sorting function.
template <typename SortFunc, typename... Args> // variadic template
long long MeasureSortTime(std::vector<int> vec, SortFunc sort_func, Args&&... args) {
  // takes vector, function pointer and 0 -> n arguments
  const auto start = std::chrono::high_resolution_clock::now();
  sort_func(vec, std::forward<Args>(args)...); // forward to preserve lvalue/rvalue
  const auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}


int main() {
  std::vector<int> v = GenerateVector(8);
  PrintVec(v);
  std::cout << KthSmallestElem(v, 0, v.size() - 1, 2 - 1) << '\n'; //-1 for 0 indexing 

  int sizes[] = {1000, 25000, 50000, 75000, 100000};
  const std::string TimeUnit = " uS\n";
  for (const int size : sizes) {
    const std::vector<int> vec = GenerateVector(size);
    std::cout << "Size: " << size << "\n";
    std::cout << "QuickSort: " << MeasureSortTime(vec, QuickSort, 0, size - 1) << TimeUnit;
    std::cout << "HeapSort: " << MeasureSortTime(vec, HeapSort) << TimeUnit;
    std::cout << "HybridSort: " << MeasureSortTime(vec, HybridMerge, 0, size - 1, 32) << TimeUnit;
    std::cout << "MergeSort: " << MeasureSortTime(vec, MergeSort, 0, size - 1) << TimeUnit;
    std::cout << "InsertionSort: " << MeasureSortTime(vec, InsertionSort, 0, size - 1) << TimeUnit;
    std::cout << "SelectionSort: " << MeasureSortTime(vec, SelectionSort) << TimeUnit;
    std::cout << "BubbleSort: " << MeasureSortTime(vec, BubbleSort) << TimeUnit;
  }
  return 0;
}
