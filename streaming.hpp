#pragma once
#include <string>

void split_streaming(
    const std::string& pr,
    int budas,
    int rikiavimo_pasirinkimas,
    long long* out_read_ms,
    long long* out_split_ms,
    long long* out_write_ms
);
