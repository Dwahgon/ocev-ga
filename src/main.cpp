#include "ga/ga.h"
#include "ga/selection.h"
#include "ga/crossover.h"
#include "ga/mutation.h"
#include "ga/fitness.h"

#include "utils/confparser.h"
#include "utils/stringifiers.h"
#include "sat/dimacsparser.h"
#include "functions/functions.h"

#include <iostream>
#include <float.h>

enum RepresentationEnum {
    BINARY,
    INTEGER,
    INTEGER_PERM,
    REAL
};
RepresentationEnum stringToRepresentationEnum(std::string const& s){
    if(s == "binary") return BINARY;
    if(s == "integer") return INTEGER;
    if(s == "integer_perm") return INTEGER_PERM;
    if(s == "real") return REAL;
    std::cout << "Invalid representation: " << s;
    exit(1);
}

enum SelectionEnum {
    ROULETTE
};
SelectionEnum stringToSelectionEnum(std::string const& s){
    if(s == "roulette") return ROULETTE;
    std::cout << "Invalid selection function: " << s;
    exit(1);
}

enum CrossoverEnum {
    ONE_POINT
};
CrossoverEnum stringToCrossoverEnum(std::string const& s){
    if(s == "onepoint") return ONE_POINT;
    std::cout << "Invalid crossover function: " << s;
    exit(1);
}

enum MutationEnum {
    BITWISE
};
MutationEnum stringToMutationEnum(std::string const& s){
    if(s == "bitwise") return BITWISE;
    std::cout << "Invalid mutation function: " << s;
    exit(1);
}

enum ObjectiveEnum {
    FUNC1,
    SAT,
};
ObjectiveEnum stringToObjectiveEnum(std::string const& s){
    if(s == "func1") return FUNC1;
    if(s == "sat") return SAT;
    std::cout << "Invalid objective function: " << s;
    exit(1);
}

enum FitnessEnum {
    EQ_OBJECTIVE,
    BIN_TO_REAL,
};
FitnessEnum stringToFitnessEnum(std::string const& s){
    if(s == "bintoreal") return BIN_TO_REAL;
    if(s == "eqobjective") return EQ_OBJECTIVE;
    std::cout << "Invalid fitness function: " << s;
    exit(1);
}

int main(int argc, char* argv[])
{
    std::string confPath {argc > 1 ? argv[1] : "conf.conf"};

    // Load conf
    confmap confs {parseConf(confPath)};
    std::size_t     dim             {std::stoul(confs.at("DIM"))},
                    pop             {std::stoul(confs.at("POP"))},
                    precision       {confs.count("PRECISION") ? std::stoul(confs.at("PRECISION")) : 0},
                    generations     {std::stoul(confs.at("GENERATIONS"))};
    std::string     representation  {confs.at("REPRESENTATION")},
                    selection       {confs.at("SELECTION")},
                    crossover       {confs.at("CROSSOVER")},
                    mutation        {confs.at("MUTATION")},
                    objective       {confs.at("OBJECTIVE")},
                    fitness         {confs.at("FITNESS")};
    double          crossoverRate   {std::stod(confs.at("CROSSOVER_RATE"))},
                    mutationRate    {std::stod(confs.at("MUTATION_RATE"))},
                    C               {confs.count("C") ? std::stod(confs.at("C")) : 0.0};
    bool            maximize        {confs.at("MAXIMIZE") == "true"};
    unsigned long   seed            {confs.count("SEED") && confs.at("SEED").length() ? std::stoul(confs.at("SEED")) : std::random_device()()};
    [[maybe_unused]]
    ga::GeneInt     istart          {std::stoi(confs.count("INT_RANGE_START")   ? confs.at("INT_RANGE_START")   : std::to_string(INT32_MIN))},
                    iend            {std::stoi(confs.count("INT_RANGE_END")     ? confs.at("INT_RANGE_END")     : std::to_string(INT32_MAX))};
    [[maybe_unused]]
    ga::GeneReal    rstart          {std::stod(confs.count("REAL_RANGE_START")  ? confs.at("REAL_RANGE_START")  : std::to_string(DBL_MIN))},
                    rend            {std::stod(confs.count("REAL_RANGE_END")    ? confs.at("REAL_RANGE_END")    : std::to_string(DBL_MAX))};

    // Configure selection function
    ga::SelectionFunction selectionFunc;
    switch (stringToSelectionEnum(selection))
    {
        case ROULETTE:
            selectionFunc = ga::rouletteWheelSelection;
            break;
    }

    // Configure GA
    switch (stringToRepresentationEnum(representation))
    {
    case BINARY: {
        // Configure fitness function
        ga::FitnessFunction<ga::GeneBin> fitnessFunc;
        sat::Formula formula;
        switch (stringToFitnessEnum(fitness))
        {
        case BIN_TO_REAL: {
            // Configure objective function
            std::function<double(std::vector<double>)> objectiveFunc;
            switch (stringToObjectiveEnum(objective))
            {
            case FUNC1:{
                objectiveFunc = [](std::vector<double>xs) {return func1(xs.at(0));};
                break;
            }
            default:
                std::cout << "Fitness function " << fitness << " with objective function" << objective << " is not supported" << std::endl;
                exit(1);
            }

            dim = ga::calculateBinToRealBitSize(rstart, rend, precision);
            fitnessFunc = ga::createBinToRealFitness(objectiveFunc, maximize, dim, rstart, rend, precision, C);

            break;
        } case EQ_OBJECTIVE: {
            // Configure objective function
            std::function<ga::Score(ga::Chromosome<ga::GeneBin>)> objectiveFunc;
            switch (stringToObjectiveEnum(objective))
            {
            case SAT:{
                formula = parseDimacsFormula(std::cin);
                objectiveFunc = std::bind(&sat::Formula::score, &formula, std::placeholders::_1);
                break;
            }
            default:{
                std::cout << "Fitness function " << fitness << " with objective function" << objective << " is not supported" << std::endl;
                exit(1);
            }
            }
            fitnessFunc = objectiveFunc;
        }
        }

        // Configure crossover function
        ga::CrossoverFunction<ga::GeneBin> crossoverFunc;
        switch (stringToCrossoverEnum(crossover))
        {
            case ONE_POINT:
                crossoverFunc = std::bind(ga::binOnePointCrossover, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, dim);
                break;
        }

        // Configure muatation function
        ga::MutationFunction<ga::GeneBin> mutationFunc;
        switch (stringToMutationEnum(mutation))
        {
            case BITWISE:
                mutationFunc = std::bind(ga::bitwiseMutation, std::placeholders::_1, std::placeholders::_2, dim, std::placeholders::_3);
                break;
        }

        // Configure population generator
        ga::ChromosomeGenerator cg {dim, seed};
        ga::PopulationGenerator<ga::GeneBin> popgen {std::bind(ga::generateGeneBinPopulation, std::ref(cg), std::placeholders::_1)};

        // Configure GA
        ga::GeneticAlgorithm<ga::GeneBin> geneticAlgorithm {
            seed,
            pop,
            popgen,
            fitnessFunc,
            selectionFunc,
            crossoverFunc,
            mutationFunc,
            crossoverRate,
            mutationRate
        };

        // Print conf
        std::cout   << "POP: "              << pop              << '\n'
                    << "DIM: "              << dim              << '\n'
                    << "GENERATIONS: "      << generations      << '\n'
                    << "OBJECTIVE: "        << objective        << '\n'
                    << "FITNESS: "          << fitness          << '\n'
                    << "SELECTION: "        << selection        << '\n'
                    << "CROSSOVER: "        << crossover        << '\n'
                    << "CROSSOVER RATE: "   << crossoverRate    << '\n'
                    << "MUTATION: "         << mutation         << '\n'
                    << "MUTATION RATE: "    << mutationRate     << '\n'
                    << "PRECISION: "        << precision        << '\n'
                    << "C: "                << C                << '\n'
                    << "MAXIMIZE: "         << maximize         << '\n'
                    << "SEED: "             << seed             << std::endl;

        // Start population
        geneticAlgorithm.initPopulation();

        // Run evolutionary loop
        for(std::size_t i = 0; i < generations; i++){
            geneticAlgorithm.step();
            std::cout << gaGenerationScoreInfoToString(geneticAlgorithm) << std::endl;
        }
        std::cout << gaSolutionToString(geneticAlgorithm, dim) << std::endl;
        break;
    }
    default: {
        std::cout << "Representation '" << representation  <<"' not supported yet" << std::endl;
        exit(1);
        break;
    }
    }
}