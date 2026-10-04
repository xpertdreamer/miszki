#include "math.h"
#include "thirdparty/argparse.hpp"
#include "util.h"

#include <iostream>
#include <vector>

int
main(int argc, char* argv[])
{
    argparse::ArgumentParser program("lab3");

    argparse::ArgumentParser euler_cmd("euler");
    euler_cmd.add_description("Get Euler's totient function (phi(x)) from number");
    euler_cmd.add_argument("number")
        .scan<'u', unsigned>()
        .required();

    argparse::ArgumentParser gcd_cmd("gcd");
    gcd_cmd.add_description("Extended Euclidian algorithm");
    gcd_cmd.add_argument("a").scan<'u', u64>().required();
    gcd_cmd.add_argument("b").scan<'u', u64>().required();

    argparse::ArgumentParser crt_cmd("crt");
    crt_cmd.add_description("Get N by Chinese Remainder Theorem");
    crt_cmd.add_argument("-r", "--rem")
        .help("List of remainders")
        .scan<'u', u64>()
        .metavar("rem1 rem2")
        .nargs(argparse::nargs_pattern::any)
        .required();
    crt_cmd.add_argument("-m", "--mod")
        .help("List of moduli")
        .scan<'u', u64>()
        .metavar("mod1 mod2")
        .nargs(argparse::nargs_pattern::any)
        .required();

    program.add_subparser(euler_cmd);
    program.add_subparser(gcd_cmd);
    program.add_subparser(crt_cmd);

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    if (program.is_subcommand_used("euler")) {
        unsigned num = euler_cmd.get<unsigned>("number");
        u64 result = num < SIEVE_LIMIT ? math::euler_sieve(num) : math::euler_bruteforce(num);
        std::cout << "phi(" << num << ") = " << result << std::endl;
        return 0;
    }

    if (program.is_subcommand_used("gcd")) {
        u64 a = gcd_cmd.get<u64>("a");
        u64 b = gcd_cmd.get<u64>("b");
        if (a > 2*1e62 || b > 2*1e62) {
            ERROR("Numbers with this size cant be processed");
            return 1;
        }
        auto x = math::extended_gcd(a, b);
        std::cout << "gcd(" << a << ", " << b << ") = "
                  << x.first << " = (" << x.second.first << " * " << a
                  << ") + (" << x.second.second  << " * " << b << ")" << std::endl;
        return 0;
    }

    if (program.is_subcommand_used("crt")) {
        auto remainders = crt_cmd.get<std::vector<u64>>("--rem");
        auto moduli = crt_cmd.get<std::vector<u64>>("--mod");
        if (remainders.size() != moduli.size()) {
            ERROR("Count of remainders must match count of moduli");
            return 1;
        }
        if (remainders.empty()) {
            ERROR("No equations provided for CRT");
            return 1;
        }
        u64 result = math::crt(moduli, remainders);
        if (result != 0) {
            std::cout << "Result N = " << result << std::endl;
            return 0;
        }
        ERROR("Cannot find N. The sizes of the vectors do not match, or you have incompatible numbers");
        return 1;
    }

    ERROR("No action specified. Use lab3 --help for help");
    return 1;
}
