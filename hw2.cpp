#include <iostream>
#include <memory>
#include <chrono>
#include <vector>

// ==========================================
// Task 1: Raw Dynamic Allocation (RawBuffer)
// ==========================================
class RawBuffer {
private:
    int* data;
    size_t size;

public:
    RawBuffer(size_t n) : size(n) {
        data = new int[n];
    }

    ~RawBuffer() {
        delete[] data;
    }

    // 복사 방지 (Copy operations disabled)
    RawBuffer(const RawBuffer&) = delete;
    RawBuffer& operator=(const RawBuffer&) = delete;

    int* get() const { return data; }
    size_t getSize() const { return size; }
};

// ==========================================
// Task 2: Smart Pointer RAII Guard (SmartBuffer)
// ==========================================
class SmartBuffer {
private:
    std::unique_ptr<int[]> data;
    size_t size;

public:
    SmartBuffer(size_t n) : size(n) {
        data = std::make_unique<int[]>(n);
    }

    // RAII로 인한 자동 소멸 (수동 delete[] 불필요)
    ~SmartBuffer() = default;

    // unique_ptr의 단독 소유권 보장을 위해 복사 금지
    SmartBuffer(const SmartBuffer&) = delete;
    SmartBuffer& operator=(const SmartBuffer&) = delete;

    // 이동 연산 허용 (Move semantics)
    SmartBuffer(SmartBuffer&&) = default;
    SmartBuffer& operator=(SmartBuffer&&) = default;

    int* get() const { return data.get(); }
    size_t getSize() const { return size; }
};

// ==========================================
// Task 3: RAII Timer Guard
// ==========================================
class Timer {
private:
    std::string name;
    std::chrono::high_resolution_clock::time_point start;

public:
    Timer(const std::string& timerName) 
        : name(timerName), start(std::chrono::high_resolution_clock::now()) {}

    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "[" << name << "] Execution Time: " << duration << " us" << std::endl;
    }

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
};

// ==========================================
// Task 4: Smart Pointer Benchmark
// ==========================================
void runBenchmark() {
    const size_t iterations = 500000;
    std::cout << "\n--- Running Benchmarks (" << iterations << " iterations) ---" << std::endl;

    {
        Timer t("std::unique_ptr Creation");
        for (size_t i = 0; i < iterations; ++i) {
            auto ptr = std::make_unique<int>(42);
        }
    }

    {
        Timer t("std::shared_ptr Creation");
        for (size_t i = 0; i < iterations; ++i) {
            auto ptr = std::make_shared<int>(42);
        }
    }

    {
        auto basePtr = std::make_shared<int>(42);
        Timer t("std::shared_ptr Copy (Refcount Modification)");
        for (size_t i = 0; i < iterations; ++i) {
            std::shared_ptr<int> copyPtr = basePtr; // 원자적 참조 카운트 증가/감소 발생
        }
    }
}

int main() {
    std::cout << "Starting HW2 Memory Management & Smart Pointers Test..." << std::endl;

    // Raw & Smart Buffer 동작 검증
    RawBuffer rawBuf(100);
    SmartBuffer smartBuf(100);

    std::cout << "RawBuffer size: " << rawBuf.getSize() << std::endl;
    std::cout << "SmartBuffer size: " << smartBuf.getSize() << std::endl;

    // 벤치마크 실행
    runBenchmark();

    return 0;
}