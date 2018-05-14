// This file is part of Lattice Builder.
//
// Copyright (C) 2012-2016  Pierre L'Ecuyer and Universite de Montreal
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
#include "netbuilder/LevelCombiner.h"

#include "netbuilder/FigureOfMerit/FigureOfMerit.h"
#include "netbuilder/FigureOfMerit/CombinedFigureOfMerit.h"
#include "netbuilder/FigureOfMerit/EquidistributionProperty.h"
#include "netbuilder/FigureOfMerit/TValueProjMerit.h"
#include "netbuilder/JoeKuoWeights.h"

#include "netbuilder/Task/CBCSearch.h"
#include "netbuilder/Task/FullCBCExplorer.h"

#include <iostream>
#include <memory>
#include <limits>



using namespace NetBuilder;

int main(int argc, const char *argv[])
{
    unsigned int s = 10;
    unsigned int m = 31;

    auto weights = std::make_unique<NetBuilder::JoeKuoWeights>();

    // auto fig1 = std::make_unique<FigureOfMerit::AProperty>();

    auto projDep = std::make_unique<FigureOfMerit::TValueProjMerit<PointSetType::MULTILEVEL>>(2, JoeKuoD6Combiner());
    auto fig2 = std::make_unique<FigureOfMerit::WeightedFigureOfMerit<FigureOfMerit::TValueProjMerit<PointSetType::MULTILEVEL>>>(std::numeric_limits<Real>::infinity(), std::move(weights), std::move(projDep));

    // std::vector<std::unique_ptr<FigureOfMerit::FigureOfMerit>> figures;
    
    // figures.push_back(std::move(fig1));
    // figures.push_back(std::move(fig2));

    // std::vector<Real> weightsFigure {1,1};

    // auto fig = std::make_unique<FigureOfMerit::CombinedFigureOfMerit>(1,std::move(figures),weightsFigure);
    
    auto explorer = std::make_unique<Task::FullCBCExplorer<NetConstruction::SOBOL>>(s,m);
    auto task = Task::CBCSearch<NetConstruction::SOBOL,Task::FullCBCExplorer>(s,m, std::move(fig2), std::move(explorer),5);

    task.execute();

    if (task.hasFoundNet())
    {
        std::cout << "Search was successful." << std::endl;
        std::cout << "Selected net: "<< std::endl;
        std::cout <<  task.outputNet(OutputFormat::CLI) << std::endl;

        std::cout << "Merit value: " << task.outputMeritValue() << std::endl;
    }

    return 0;
}
