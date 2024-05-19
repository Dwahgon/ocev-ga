#include "main.h"

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
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))


std::size_t dim, pop, generationGap, precision, generations, tournamentK;
std::string representation, selection, crossover, mutation, objective, linearScalingFuncStr, steadyStateFuncStr;
double crossoverRate, mutationRate, worstCaseOffset, inequalityPenalty, equalityPenalty, tournamentKp;
bool maximize, elitism, linearScalingEnabled;
int threads;
unsigned long seed;
ga::GeneInt istart, iend;
ga::GeneReal rstart, rend;
ga::SelectionFunction selectionFunc;
ga::EasingFunction linearScalingFunc, steadyStateFunc;

RepresentationEnum stringToRepresentationEnum(std::string const& s){
    if(s == "binary") return BINARY;
    if(s == "integer") return INTEGER;
    if(s == "integer_perm") return INTEGER_PERM;
    if(s == "real") return REAL;
    std::cout << "Invalid representation: " << s;
    exit(1);
}

SelectionEnum stringToSelectionEnum(std::string const& s){
    if(s == "roulette") return ROULETTE;
    if(s == "tournament") return TOURNAMENT;
    std::cout << "Invalid selection function: " << s;
    exit(1);
}

CrossoverEnum stringToCrossoverEnum(std::string const& s){
    if(s == "onepoint") return ONE_POINT;
    if(s == "uniform") return UNIFORM;
    if(s == "pmx") return PMX;
    if(s == "cx") return CX;
    std::cout << "Invalid crossover function: " << s;
    exit(1);
}

MutationEnum stringToMutationEnum(std::string const& s){
    if(s == "bitwise") return BITWISE;
    if(s == "swap") return SWAP;
    std::cout << "Invalid mutation function: " << s;
    exit(1);
}

ObjectiveEnum stringToObjectiveEnum(std::string const& s){
    if(s == "func1") return FUNC1;
    if(s == "sat") return SAT;
    if(s == "radio") return RADIO;
    if(s == "nqueens") return NQUEENS;
    std::cout << "Invalid objective function: " << s;
    exit(1);
}

LinearScalingEnum stringToLinearScalingEnum(std::string const& s){
    if(s == "none") return LS_NONE;
    if(s == "linear") return LINEAR;
    if(s == "easeinoutcubic") return EASE_IN_OUT_CUBIC;
    std::cout << "Invalid linear scaling function: " << s;
    exit(1);
}

SteadyStateEnum stringToSteadyStateEnum(std::string const& s){
    if(s == "none") return SS_NONE;
    if(s == "uniformsteps") return UNIFORM_STEPS;
    std::cout << "Invalid steady step function: " << s;
    exit(1);
}

template <class T>
void evolutionaryLoop(ga::GeneticAlgorithm<T> &geneticAlgorithm, std::string outPath){
    // Print conf
    std::cout   << "POP: "                  << pop                  << '\n'
                << "DIM: "                  << dim                  << '\n'
                << "GENERATIONS: "          << generations          << '\n'
                << "GENERATION GAP: "       << generationGap        << '\n'
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
                << "TOURNAMENT_KP: "        << tournamentKp         << '\n'
                << "TOURNAMENT_K: "         << tournamentK          << '\n'
                << "MAXIMIZE: "             << maximize             << '\n'
                << "THREADS: "              << threads              << '\n'
                << "SEED: "                 << seed                 << std::endl;

    // Start population
    geneticAlgorithm.initPopulation();

    // Run evolutionary loop
    for(std::size_t i = 0; i < generations; i++){
        geneticAlgorithm.step();
        std::cout << gaGenerationScoreInfoToString(geneticAlgorithm) << std::endl;
    }
    std::cout << gaSolutionToString(geneticAlgorithm, 0, worstCaseOffset) << std::endl;

    std::ofstream out(outPath);
    out << gaConvergenceTableToString(geneticAlgorithm);
    out.close();
}
template void evolutionaryLoop(ga::GeneticAlgorithm<ga::GeneBin> &geneticAlgorithm, std::string outPath);
template void evolutionaryLoop(ga::GeneticAlgorithm<ga::GeneIntPerm> &geneticAlgorithm, std::string outPath);

void runGeneBinGA(std::string outPath){
    // Configure GA
    ga::BinaryToNumericConversionFitness<double>* binToDoubleConversionFitness = NULL;
    ga::BinaryToNumericConversionFitness<unsigned int>* binToUIConversionFitness = NULL;
    sat::Formula *formula = NULL;
    ga::FitnessFunction<ga::GeneBin> fitnessFunc;

    switch (stringToObjectiveEnum(objective))
        {
        case SAT:{
            formula = new sat::Formula(parseDimacsFormula(std::cin));
            dim = formula->getVarCount();
            fitnessFunc = [formula](ga::Chromosome<ga::GeneBin> chromosome){return (double)formula->score(chromosome) / (double)formula->getClauseCount();};
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
            break;
        default:
            std::cout << "Crossover function not supported for binary representations: " << crossover << std::endl;
    }

    // Configure muatation function
    ga::MutationFunction<ga::GeneBin> mutationFunc;
    switch (stringToMutationEnum(mutation))
    {
        case BITWISE:
            mutationFunc = std::bind(ga::bitwiseMutation, std::placeholders::_1, std::placeholders::_2, dim, std::placeholders::_3);
            break;
        default:
            std::cout << "Mutation function not supported for binary representations: " << mutation << std::endl;
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
        steadyStateFunc,
        linearScalingFunc,
        linearScalingEnabled,
        crossoverRate,
        mutationRate,
        elitism,
        (short)threads
    };

    evolutionaryLoop(geneticAlgorithm, outPath);
    std::cout << chromosomeToString(geneticAlgorithm.getSolution().chromosome, dim) << std::endl;

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
    }default: break;
    }

    delete binToDoubleConversionFitness;
    delete binToUIConversionFitness;
    delete formula;
}

void runGeneIntPermGA(std::string outPath){
    ga::FitnessFunction<ga::GeneIntPerm> fitnessFunc;
    switch (stringToObjectiveEnum(objective))
    {
    case NQUEENS:{
        std::size_t maxCollisions = dim * (dim - 1);
        fitnessFunc = [maxCollisions](const ga::Chromosome<ga::GeneIntPerm> &chromosome) {
            return (double)(maxCollisions - nQueens(chromosome))/(double)maxCollisions;
        };
        break;
    }default: {
        std::cout << "Objective not supported for integer permutation representations: " << objective << std::endl;
    }
    }

    // Configure crossover function
    ga::CrossoverFunction<ga::GeneIntPerm> crossoverFunc;
    switch (stringToCrossoverEnum(crossover))
    {
        case PMX:
            crossoverFunc = ga::pmxCrossover;
            break;
        case CX:
            crossoverFunc = ga::cxCrossover;
            break;
        default:
            std::cout << "Crossover function not supported for integer permutation representations: " << crossover << std::endl;
    }

    // Configure muatation function
    ga::MutationFunction<ga::GeneIntPerm> mutationFunc;
    switch (stringToMutationEnum(mutation))
    {
        case SWAP:
            mutationFunc = ga::swapMutation;
            break;
        default:
            std::cout << "Mutation function not supported for integer permutation representations: " << mutation << std::endl;
    }

    // Configure population generator
    ga::ChromosomeGenerator cg {dim, seed};
    ga::PopulationGenerator<ga::GeneIntPerm> popgen {std::bind(ga::generateGeneIntPermPopulation, std::ref(cg), std::placeholders::_1)};

    // Configure GA
    ga::GeneticAlgorithm<ga::GeneIntPerm> geneticAlgorithm {
        seed,
        pop,
        popgen,
        fitnessFunc,
        selectionFunc,
        crossoverFunc,
        mutationFunc,
        steadyStateFunc,
        linearScalingFunc,
        linearScalingEnabled,
        crossoverRate,
        mutationRate,
        elitism,
        (short)threads
    };

    evolutionaryLoop(geneticAlgorithm, outPath);
    std::cout << chromosomeToString(geneticAlgorithm.getSolution().chromosome, 0) << std::endl;
}

int main(int argc, char* argv[])
{
    std::string confPath {""}, outPath {""};
    bool showPlot = false;

    for (int i = 1; i < argc; i++){
        if (argv[i][0] == '-'){ // Flags
            if(argv[i][1] == '-'){ // Word flags
                showPlot = strcmp((argv[i] + 2), "show-plot") == 0 || showPlot;
            }
            else{ // Letter flags
                showPlot = strstr((argv[i] + 1), "p") != NULL || showPlot;
            }
        }else{ // Arguments
            if (!confPath.size()) confPath = argv[i];
            else if(!outPath.size()) outPath = argv[i];
        }
    }
    if (!confPath.size()) confPath = "conf.conf";
    else if(!outPath.size()) outPath = "out.csv";

    // Load conf
    confmap confs {parseConf(confPath)};
    dim                     = confs.count("DIM") ? std::stoul(confs.at("DIM")) : 0;
    pop                     = std::stoul(confs.at("POP"));
    generationGap           = confs.count("GENERATION_GAP") ? std::stoul(confs.at("GENERATION_GAP")) : 0;
    precision               = confs.count("PRECISION") ? std::stoul(confs.at("PRECISION")) : 0;
    generations             = std::stoul(confs.at("GENERATIONS"));
    tournamentK             = confs.count("TOURNAMENT_K") ? std::stoul(confs.at("TOURNAMENT_K")) : 2;
    representation          = confs.at("REPRESENTATION");
    selection               = confs.at("SELECTION");
    crossover               = confs.at("CROSSOVER");
    mutation                = confs.at("MUTATION");
    objective               = confs.at("OBJECTIVE");
    linearScalingFuncStr    = confs.count("LINEAR_SCALING_FUNC") ? confs.at("LINEAR_SCALING_FUNC") : "none";
    steadyStateFuncStr      = confs.count("STEADY_STATE_FUNC") ? confs.at("STEADY_STATE_FUNC") : "none";
    crossoverRate           = std::stod(confs.at("CROSSOVER_RATE"));
    mutationRate            = std::stod(confs.at("MUTATION_RATE"));
    worstCaseOffset         = confs.count("WORST_CASE_OFFSET") ? std::stod(confs.at("WORST_CASE_OFFSET")) : 0.0;
    inequalityPenalty       = confs.count("INEQUALITY_PENALTY") ? std::stod(confs.at("INEQUALITY_PENALTY")) : 0.0;
    equalityPenalty         = confs.count("EQUALITY_PENALTY") ? std::stod(confs.at("EQUALITY_PENALTY")) : 0.0;
    tournamentKp            = confs.count("TOURNAMENT_KP") ? std::stod(confs.at("TOURNAMENT_KP")) : 1.0;
    maximize                = confs.count("MAXIMIZE") ? confs.at("MAXIMIZE") == "true" : false;
    elitism                 = confs.at("ELITISM") == "true";
    threads                 = confs.count("THREADS") ? std::stoi(confs.at("THREADS")) : 1;
    seed                    = confs.count("SEED") && confs.at("SEED").length() ? std::stoul(confs.at("SEED")) : std::random_device()();
    istart                  = std::stoi(confs.count("INT_RANGE_START")   ? confs.at("INT_RANGE_START")   : std::to_string(INT32_MIN));
    iend                    = std::stoi(confs.count("INT_RANGE_END")     ? confs.at("INT_RANGE_END")     : std::to_string(INT32_MAX));
    rstart                  = std::stod(confs.count("REAL_RANGE_START")  ? confs.at("REAL_RANGE_START")  : std::to_string(DBL_MIN));
    rend                    = std::stod(confs.count("REAL_RANGE_END")    ? confs.at("REAL_RANGE_END")    : std::to_string(DBL_MAX));

    if (pop % 2){
        std::cout << "Population must be a pair" << std::endl;
        return 1;
    }

    // Configure selection function
    switch (stringToSelectionEnum(selection))
    {
        case ROULETTE:
            selectionFunc = ga::rouletteWheelSelection;
            break;
        case TOURNAMENT:
            selectionFunc = std::bind(ga::tournamentSelection, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, tournamentK, tournamentKp);
            break;
    }


    switch (stringToLinearScalingEnum(linearScalingFuncStr))
    {
    case LS_NONE:
        linearScalingEnabled = false;
        linearScalingFunc = [](std::size_t generation){return 0.0;};
        break;
    case LINEAR:
        linearScalingEnabled = true;
        linearScalingFunc = [](std::size_t generation){return (double)generation / (double)generations;};
        break;
    case EASE_IN_OUT_CUBIC:
        linearScalingEnabled = true;
        linearScalingFunc = [](std::size_t generation){
            double x = (double)generation / (double)generations;
            return x < 0.5 ? 4.0 * x * x * x : 1.0 - pow(-2.0 * x + 2.0, 3.0) / 2.0;
        };
        break;
    }

    switch (stringToSteadyStateEnum(steadyStateFuncStr))
    {
    case SS_NONE:
        steadyStateFunc = [](std::size_t generation){return 1.0;};
        break;
    case UNIFORM_STEPS:
        steadyStateFunc = [](std::size_t generation){
            double x = MAX(((double)generation - 1.0), 0.0) / (double)generations;
            return 0.1 + 0.1 * std::floor(x / 0.1);
        };
    }

    switch (stringToRepresentationEnum(representation))
    {
    case BINARY: runGeneBinGA(outPath); break;
    case INTEGER_PERM: runGeneIntPermGA(outPath); break;
    default: {
        std::cout << "Representation '" << representation  <<"' not supported yet" << std::endl;
        exit(1);
        break;
    }
    }

    std::string plotScript{PLOT_SCRIPT};
    system(("./" + plotScript + " " + outPath + " " + outPath + ".png" + " " + (showPlot ? "-p" : "")).c_str());

    return 0;
}