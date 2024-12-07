class PerformanceMetrics {
public:
    static void recordHashOperation(size_t input_size, 
                                  std::chrono::microseconds duration);
    static void generateReport();
private:
    static std::vector<std::pair<size_t, std::chrono::microseconds>> metrics_;
    static std::mutex metrics_mutex_;
}; 