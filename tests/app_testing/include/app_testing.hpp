#pragma once

#include <unordered_map>

namespace app_testing {
    /*--------------------------Commands------------------------*/
    enum class BinOpers : char { Add, Sub, Mul, Div };
    enum class UnOpers  : char { Init, Def, AddAss, SubAss, MulAss, DivAss, Transp, Det, Negate, Chain };

    const std::unordered_map<std::string, BinOpers> bin_operators {
        {"add", BinOpers::Add}, {"sub", BinOpers::Sub}, {"mul", BinOpers::Mul}, {"div", BinOpers::Div}
    };

    const std::unordered_map<std::string, UnOpers> un_operators {
        {"init", UnOpers::Init}, {"def", UnOpers::Def}, {"sub_ass", UnOpers::AddAss},
        {"sub_ass", UnOpers::SubAss}, {"mul_ass", UnOpers::MulAss}, {"div_ass", UnOpers::DivAss},
        {"transp", UnOpers::Transp}, {"det", UnOpers::Det}, {"neg", UnOpers::Negate}, {"chain", UnOpers::Chain}
    };
} // <--- namespace app_testing
