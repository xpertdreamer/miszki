#include "math.h"
#include "thirdparty/argparse.hpp"
#include "util.h"

#include <iostream>

constexpr char flag_euler[] = "-e";
constexpr char flag_euclid[] = "-g";

int
main(int argc, char* argv[])
{
    argparse::ArgumentParser program("lab3");
    auto& group = program.add_mutually_exclusive_group();

    group.add_argument(flag_euler, "--euler")
        .help("Get Euler's totient function (phi(x)) from number")
        .nargs(1)
        .scan<'u', unsigned>();

    group.add_argument(flag_euclid, "--gcd")
        .help("Extended Euclidian algorithm")
        .nargs(2)
        .scan<'u', u64>();

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    if (auto e_val = program.present<unsigned>(flag_euler)) {
        u64 num = *e_val;
        u64 result = num < SIEVE_LIMIT ? math::euler_sieve(num) : math::euler_bruteforce(num);
        std::cout << "phi(" << num << ") = " << result << std::endl;
        return 0;
    }

    if (program.is_used(flag_euclid)) {
        auto v = program.get<std::vector<u64>>(flag_euclid);
        if (v[0] > 2*10e62 || v[1] > 2*10e62) {
            ERROR("Numbers with this size cant be processed");
            return 1;
        }
        auto x = math::extended_gcd(v[0], v[1]);
        std::cout << "gcd(" << v[0] << ", " << v[1] << ") = "
                  << x.first << " = (" << x.second.first << " * " << v[0]
                  << ") + (" << x.second.second  << " * " << v[1] << ")" << std::endl;
        return 0;
    }

    ERROR("No action specified. Use --help for help");
    return 1;
}
