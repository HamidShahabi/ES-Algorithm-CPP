#pragma once
#include <memory>
#include <systemc.h>

#include "genetic_algorithm_types.hh"
#include "software_part.hh"

using namespace GeneticAlgorithmTypes;

SC_MODULE(PopulationUpdator)
{
  sc_in<double> p_cross_over, p_mutation;
  sc_event *do_update, *update_done;
  Population *population;

  int get_random_number(int range)
  {
    return 1 + std::rand() / ((RAND_MAX + 1u) / range);
  }

  void mutate(Chromosome & chromosome)
  {
    for (auto &gen : chromosome.gens)
      if (get_random_number(100) <= 15)
        gen = (gen == Gen::DISABLE ? Gen::ENABLE : Gen::DISABLE);
  }

  void cross_over(Chromosome & c1, Chromosome & c2)
  {
    int cross_point = get_random_number(c1.gens.size() - 1);

    for (int i = 0; i < cross_point; i++)
      std::swap(c1.gens[i], c2.gens[i]);
  }

  void update_population(Population & population)
  {
    std::sort(population.chromosomes.begin(), population.chromosomes.end(),
              [](const Chromosome &c1, const Chromosome &c2) {
                return c1.fitness > c2.fitness;
              });

    for (int i = population.chromosomes.size() % 2;
         i < population.chromosomes.size() - 1; i += 2)
      if (get_random_number(100) <= p_cross_over.read() * 100)
        cross_over(population.chromosomes[i], population.chromosomes[i + 1]);

    for (auto &chromosome : population.chromosomes)
      if (get_random_number(100) <= p_mutation.read() * 100)
        mutate(chromosome);
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