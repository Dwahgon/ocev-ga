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
#include <fstream>

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
    ONE_POINT,
    UNIFORM,
};
CrossoverEnum stringToCrossoverEnum(std::string const& s){
    if(s == "onepoint") return ONE_POINT;
    if(s == "uniform") return UNIFORM;
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
    RADIO,
};
ObjectiveEnum stringToObjectiveEnum(std::string const& s){
    if(s == "func1") return FUNC1;
    if(s == "sat") return SAT;
    if(s == "radio") return RADIO;
    std::cout << "Invalid objective function: " << s;
    exit(1);
}

int main(int argc, char* argv[])
{
    std::string confPath {argc > 1 ? argv[1] : "conf.conf"}, outPath {argc > 2 ? argv[2] : "out.csv"};

    // Load conf
    confmap confs {parseConf(confPath)};
    std::size_t     dim                 {},
                    pop                 {std::stoul(confs.at("POP"))},
                    precision           {confs.count("PRECISION") ? std::stoul(confs.at("PRECISION")) : 0},
                    generations         {std::stoul(confs.at("GENERATIONS"))};
    std::string     representation      {confs.at("REPRESENTATION")},
                    selection           {confs.at("SELECTION")},
                    crossover           {confs.at("CROSSOVER")},
                    mutation            {confs.at("MUTATION")},
                    objective           {confs.at("OBJECTIVE")};
    double          crossoverRate       {std::stod(confs.at("CROSSOVER_RATE"))},
                    mutationRate        {std::stod(confs.at("MUTATION_RATE"))},
                    worstCaseOffset     {confs.count("WORST_CASE_OFFSET") ? std::stod(confs.at("WORST_CASE_OFFSET")) : 0.0},
                    inequalityPenalty   {confs.count("INEQUALITY_PENALTY") ? std::stod(confs.at("INEQUALITY_PENALTY")) : 0.0},
                    equalityPenalty     {confs.count("EQUALITY_PENALTY") ? std::stod(confs.at("EQUALITY_PENALTY")) : 0.0};
    bool            maximize            {confs.at("MAXIMIZE") == "true"},
                    elitism             {confs.at("ELITISM") == "true"};
    unsigned long   seed                {confs.count("SEED") && confs.at("SEED").length() ? std::stoul(confs.at("SEED")) : std::random_device()()};
    [[maybe_unused]]
    ga::GeneInt     istart              {std::stoi(confs.count("INT_RANGE_START")   ? confs.at("INT_RANGE_START")   : std::to_string(INT32_MIN))},
                    iend                {std::stoi(confs.count("INT_RANGE_END")     ? confs.at("INT_RANGE_END")     : std::to_string(INT32_MAX))};
    [[maybe_unused]]
    ga::GeneReal    rstart              {std::stod(confs.count("REAL_RANGE_START")  ? confs.at("REAL_RANGE_START")  : std::to_string(DBL_MIN))},
                    rend                {std::stod(confs.count("REAL_RANGE_END")    ? confs.at("REAL_RANGE_END")    : std::to_string(DBL_MAX))};

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
        ga::BinaryToNumericConversionFitness<double>* binToDoubleConversionFitness = NULL;
        ga::BinaryToNumericConversionFitness<unsigned int>* binToUIConversionFitness = NULL;
        sat::Formula *formula = NULL;
        ga::FitnessFunction<ga::GeneBin> fitnessFunc;
        switch (stringToObjectiveEnum(objective))
        {
        case SAT:{
            formula = new sat::Formula(parseDimacsFormula(std::cin));
            dim = formula->getVarCount();
            fitnessFunc = std::bind(&sat::Formula::score, formula, std::placeholders::_1);
            break;
        }
        case FUNC1: {
            dim = ga::BinaryToNumericConversionFitness<ga::GeneReal>::binToNumericBitSize(rstart, rend, precision);
            auto objectiveFunc = [](std::vector<double>xs) {return func1(xs.at(0));};
            binToDoubleConversionFitness = new ga::BinaryToNumericConversionFitness<double>(
                ga::Fitness<double> {
                    objectiveFunc,
                    {},
                    {},
                    0.,
                    0.,
                    worstCaseOffset,
                    maximize
                },
                {{rstart, rend, precision}}
            );
            fitnessFunc = std::bind(&ga::BinaryToNumericConversionFitness<ga::GeneReal>::score, binToDoubleConversionFitness, std::placeholders::_1);
            break;
        }case RADIO: {
            dim = 10;
            auto spread = [](std::function<double(unsigned int, unsigned int)> f){
                return [f](std::vector<unsigned int>xs) {return f(xs.at(0), xs.at(1));};
            };
            binToUIConversionFitness = new ga::BinaryToNumericConversionFitness<unsigned int>(
                ga::Fitness<unsigned int> {
                    spread(radioFactory),
                    {spread(radioFactoryR5)},
                    {},
                    inequalityPenalty,
                    equalityPenalty,
                    worstCaseOffset,
                    maximize
                },
                {{0, 24, 0}, {0, 16, 0}}
            );
            fitnessFunc = std::bind(&ga::BinaryToNumericConversionFitness<unsigned int>::score, binToUIConversionFitness, std::placeholders::_1);
            break;
        }
        default: {
            std::cout << "Objective not supported for binary representations: " << objective << std::endl;
        }
        }

        // Configure crossover function
        ga::CrossoverFunction<ga::GeneBin> crossoverFunc;
        switch (stringToCrossoverEnum(crossover))
        {
            case ONE_POINT:
                crossoverFunc = std::bind(ga::binOnePointCrossover, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, dim);
                break;
            case UNIFORM:
                crossoverFunc = ga::binUniformCrossover;
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
            mutationRate,
            elitism
        };

        // Print conf
        std::cout   << "POP: "                  << pop                  << '\n'
                    << "DIM: "                  << dim                  << '\n'
                    << "GENERATIONS: "          << generations          << '\n'
                    << "OBJECTIVE: "            << objective            << '\n'
                    << "SELECTION: "            << selection            << '\n'
                    << "CROSSOVER: "            << crossover            << '\n'
                    << "CROSSOVER RATE: "       << crossoverRate        << '\n'
                    << "MUTATION: "             << mutation             << '\n'
                    << "MUTATION RATE: "        << mutationRate         << '\n'
                    << "PRECISION: "            << precision            << '\n'
                    << "WORST_CASE_OFFSET: "    << worstCaseOffset      << '\n'
                    << "INEQUALITY_PENALTY: "   << inequalityPenalty    << '\n'
                    << "EQUALITY_PENALTY: "     << equalityPenalty      << '\n'
                    << "MAXIMIZE: "             << maximize             << '\n'
                    << "SEED: "                 << seed                 << std::endl;

        // Start population
        geneticAlgorithm.initPopulation();

        // Run evolutionary loop
        for(std::size_t i = 0; i < generations; i++){
            geneticAlgorithm.step();
            std::cout << gaGenerationScoreInfoToString(geneticAlgorithm) << std::endl;
        }
        std::cout << gaSolutionToString(geneticAlgorithm, dim, worstCaseOffset) << std::endl;
        ga::GeneticAlgorithmSolution<ga::GeneBin> solution {geneticAlgorithm.getSolution()};
        switch (stringToObjectiveEnum(objective))
        {
        case RADIO:{
            unsigned int st {ga::BinaryToNumericConversionFitness<ga::GeneBin>::binaryToRangeValue(solution.chromosome.at(0) & 0b11111, 5, {0, 24, 0})},
                lx {ga::BinaryToNumericConversionFitness<ga::GeneBin>::binaryToRangeValue((solution.chromosome.at(0) & (0b11111 << 5)) >> 5, 5, {0, 16, 0})};

            std::cout   << "ST: "   << st                               << '\n'
                        << "LX: "   << lx                               << '\n'
                        << "OBJ: "  << radioFactory(st,lx)*1360.0       << '\n'
                        << "RES?: " << (radioFactoryR5(st, lx) > 0)     << '\n';
            break;
        }default:{
            break;
        }
        }

        std::ofstream out(outPath);
        out << gaConvergenceTableToString(geneticAlgorithm);
        out.close();

        delete binToDoubleConversionFitness;
        delete binToUIConversionFitness;
        delete formula;
        break;
    }
    default: {
        std::cout << "Representation '" << representation  <<"' not supported yet" << std::endl;
        exit(1);
        break;
    }
    }
}