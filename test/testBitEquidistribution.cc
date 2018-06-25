// This file is part of LatNet Builder.
//
// Copyright (C) 2012-2018  Pierre L'Ecuyer and Universite de Montreal
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "netbuilder/Types.h"
#include "netbuilder/Util.h"
#include "netbuilder/DigitalNet.h"
#include "netbuilder/NetConstructionTraits.h"
#include "netbuilder/ProgressiveRowReducer.h"


#include "netbuilder/FigureOfMerit/FigureOfMerit.h"
#include "netbuilder/FigureOfMerit/BitEquidistribution.h"

#include "netbuilder/Task/Eval.h"

#include "netbuilder/LevelCombiner.h"

#include <iostream>
#include <algorithm>
#include <limits>

#include <boost/numeric/ublas/blas.hpp>

using namespace NetBuilder;

int main(int argc, const char *argv[])
{
    unsigned int s = 10;
    unsigned int m = 2*s;

    auto net = std::make_unique<DigitalNetConstruction<NetConstruction::SOBOL>>(s,m);

    // auto fig = std::make_unique<FigureOfMerit::BitEquidistribution<EmbeddingType::UNILEVEL>>(2);
    auto fig = std::make_unique<FigureOfMerit::BitEquidistribution<EmbeddingType::MULTILEVEL>>(1, std::numeric_limits<Real>::infinity(), std::numeric_limits<Real>::infinity(), LevelCombiner::MaxCombiner());

    auto task = Task::Eval(std::move(net),std::move(fig), 5);

    task.execute();
    
    return 0;
}