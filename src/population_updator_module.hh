#pragma once
#include <memory>
#include <systemc.h>

#include "genetic_algorithm_types.hh"
#include "software_part.hh"

using namespace GeneticAlgorithmTypes;

SC_MODULE(PopulationUpdator)
{
  sc_in<double> probability_cross_over, probability_mutation;
  sc_event *do_update, *update_done;
  Population *population;

  // Function to generate a random integer between 0 and range-1
  int get_random_number(int range) { return std::rand() % range; }

  // Function to generate a random float between 0.0 and 1.0
  float get_random_float()
  {
    return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
  }

  // Function to generate a random float within a specific range [min, max]
  float get_random_float_in_range(float min, float max)
  {
    return min + (static_cast<float>(std::rand()) /
                  (static_cast<float>(RAND_MAX / (max - min))));
  } // todo remove this function if not used

  float gaussianRandom()
  {
    float u, v, s;
    do
    {
      u = 2.0 * ((float)rand() / RAND_MAX) - 1.0;
      v = 2.0 * ((float)rand() / RAND_MAX) - 1.0;
      s = u * u + v * v;
    } while (s >= 1.0 || s == 0);
    float multiplier = sqrt(-2.0 * log(s) / s);
    return u * multiplier; // Just use one of the values
  }

  // Mutation Logic adapted for the Chromosome structure
  void mutate(Population & population, std::vector<float> & stdDeviation)
  {
    srand(time(NULL)); // Ideally, this should be called once elsewhere

    for (auto &chromosome : population.chromosomes)
    {
      for (size_t j = 0; j < chromosome.gens.size(); j++)
      {
        if (get_random_number(100) <= 15)
        {

          float N = gaussianRandom(); // Gaussian distributed N
          chromosome.gens[j] +=
              stdDeviation[j] * gaussianRandom(); // Mutate gene

          // Adapt stdDeviation[j] if needed based on your ES strategy
          // This is a simplistic adaptation; you might want more sophisticated
          // logic
          stdDeviation[j] *= exp(N - gaussianRandom());
          chromosome.gens[j] = repair_gen(chromosome.gens[j], 0.1);
        }
      }
    }
  }
  // Function to repair a gene value, ensuring it's not negative.
  // It clamps the value to a specified lower_bound if it's negative.
  float repair_gen(float genValue, float lower_bound)
  {
    if (genValue < 0)
    {
      return lower_bound; // Clamp to lower_bound if negative.
    }
    return genValue; // Return the original value if it's not negative.
  }

  void cross_over(const Chromosome &c1, const Chromosome &c2,
                  std::vector<Chromosome> &chromosomes)
  {
    // Ensure there is more than one gene to perform crossover
    if (c1.gens.size() < 2)
      return;

    // Generate a random crossover point, ensuring it's within bounds and not at
    // the extremities
    int cross_point = get_random_number(c1.gens.size() - 1);

    // Create two new chromosomes as children
    Chromosome child1, child2;

    // Copy genes from parents to children up to the crossover point
    child1.gens.insert(child1.gens.end(), c1.gens.begin(),
                       c1.gens.begin() + cross_point);
    child2.gens.insert(child2.gens.end(), c2.gens.begin(),
                       c2.gens.begin() + cross_point);

    // Swap the remaining genes
    child1.gens.insert(child1.gens.end(), c2.gens.begin() + cross_point,
                       c2.gens.end());
    child2.gens.insert(child2.gens.end(), c1.gens.begin() + cross_point,
                       c1.gens.end());

    // Append the new children to the chromosomes vector
    chromosomes.push_back(child1);
    chromosomes.push_back(child2);
  }

  void update_population(Population & population)
  {
    std::sort(population.chromosomes.begin(), population.chromosomes.end(),
              [](const Chromosome &c1, const Chromosome &c2)
              { return c1.fitness > c2.fitness; });

    for (int i = population.chromosomes.size() % 2;
         i < population.chromosomes.size() - 1; i += 2)
      if (get_random_number(100) <= probability_cross_over.read() * 100)
        cross_over(population.chromosomes[i], population.chromosomes[i + 1],
                   population.chromosomes);

    for (auto &chromosome : population.chromosomes)
      if (get_random_number(100) <= probability_mutation.read() * 100)
        mutate(chromosome);

    std::sort(population.chromosomes.begin(), population.chromosomes.end(),
              [](const Chromosome &c1, const Chromosome &c2)
              { return c1.fitness > c2.fitness; });
    chromosomes.erase(chromosomes.end() - 2, chromosomes.end());
  }

  void update_thread()
  {
    while (true)
    {
      wait(*do_update);
      update_population(*population);
      update_done->notify();
    }
  }

  SC_CTOR(PopulationUpdator)
  {
    std::srand(
        std::time(nullptr)); // use current time as seed for random generator
    SC_THREAD(update_thread);
  }
};