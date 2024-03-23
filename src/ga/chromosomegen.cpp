#include "chromosomegen.h"

#include <cctype>
#include <vector>
#include <algorithm>

using namespace ga;

ChromosomeGenerator::ChromosomeGenerator(const unsigned long chromossomeSize) : seed(std::random_device()()), rng(std::mt19937(this->seed)), chromossomeSize(chromossomeSize) {}

ChromosomeGenerator::ChromosomeGenerator(const unsigned long chromossomeSize, const unsigned long seed) : seed(seed), rng(std::mt19937(seed)), chromossomeSize(chromossomeSize) {}

unsigned int ChromosomeGenerator::getSeed() const
{
    return this->seed;
}

Chromosome<GeneBin> ChromosomeGenerator::generateChromosomeBin()
{
    return this->generateChromosomeBin(this->chromossomeSize);
}

Chromosome<GeneBin> ChromosomeGenerator::generateChromosomeBin(std::size_t size)
{
    const std::size_t vecsize = (size - 1) / 32 + 1; // Subtrack size by 1 because, for example, if we have a chromossome size of 32, we would only need 1 32bit integer in the vector, so (32-1) / 32 + 1 = 1. If we have a chromossome size of 33, we would need 2 32bit integer in the vector, so (33-1) / 32 + 1 = 2.
    Chromosome<GeneBin> binum(vecsize);
    std::uniform_int_distribution<GeneBin> fullBinDist(0, 0xffffffff), parcialBinDist(0, 1 << (size - (vecsize - 1) * 32));
    for (std::size_t i = 0; i < vecsize; i++ )
    {
        binum[i] = i == vecsize - 1 ? parcialBinDist(this->rng) : fullBinDist(this->rng);
    }
    return binum;
}

Chromosome<GeneInt> ChromosomeGenerator::generateChromosomeInt(const int rangeStart, const int rangeEnd)
{
    return this->generateChromosomeInt(this->chromossomeSize, rangeStart, rangeEnd);
}

Chromosome<GeneInt> ChromosomeGenerator::generateChromosomeInt(const std::size_t size, const int rangeStart, const int rangeEnd)
{
    Chromosome<GeneInt> nums(size);
    std::uniform_int_distribution<GeneInt> dist(rangeStart, rangeEnd);
    for (std::size_t i = 0; i < size; i++)
        nums[i] = dist(this->rng);
    return nums;
}

Chromosome<GeneIntPerm> ChromosomeGenerator::generateChromosomeIntPerm()
{
    return this->generateChromosomeIntPerm(this->chromossomeSize);
}

std::vector<uint32_t> ChromosomeGenerator::generateChromosomeIntPerm(const std::size_t size)
{
    std::vector<uint32_t> nums(size);
    for (std::size_t i = 0; i < size; i++)
        nums[i] = i;
    std::shuffle(nums.begin(), nums.end(), this->rng);
    return nums;
}

Chromosome<GeneReal> ChromosomeGenerator::generateChromosomeReal(const double rangeStart, const double rangeEnd)
{
    return this->generateChromosomeReal(this->chromossomeSize, rangeStart, rangeEnd);
}

Chromosome<GeneReal> ChromosomeGenerator::generateChromosomeReal(const std::size_t size, const double rangeStart, const double rangeEnd)
{
    Chromosome<GeneReal> nums(size);
    std::uniform_real_distribution<GeneReal> dist(rangeStart, rangeEnd);
    for (std::size_t i = 0; i < size; i++)
        nums[i] = dist(this->rng);
    return nums;
}