#pragma once
#include <algorithm>
#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

namespace GeneticAlgorithmTypes
{
struct Gen
{
  float existence_prob;
};

struct Chromosome
{
  std::vector<Gen> gens;
  double fitness;

  Chromosome() : gens(), fitness(0) {}

  bool operator==(const Chromosome &other) const
  {
    return gens == other.gens && fitness == other.fitness;
  }

  friend std::ostream &operator<<(std::ostream &os, const Chromosome &c)
  {
    std::cout << "[";
    for (const auto &gen : c.gens)
      std::cout << (gen == Gen::ENABLE ? "1, " : "0, ");
    std::cout << "] : " << c.fitness << std::endl;

    return os;
  }
};

struct Population
{
  std::vector<Chromosome> chromosomes;

  bool operator==(const Population &other) const
  {
    return chromosomes == other.chromosomes;
  };

  friend std::ostream &operator<<(std::ostream &os, const Population &p)
  {
    for (const auto &chromosome : p.chromosomes)
      os << chromosome;
    std::cout << std::endl;

    return os;
  }
};

struct Item
{
  int value, weight;

  Item(int i_v, int i_w) : value(i_v), weight(i_w) {}

  bool operator==(const Item &other) const
  {
    return value == other.value && weight == other.weight;
  };

  friend std::ostream &operator<<(std::ostream &os, const Item &i)
  {
    os << i.value << ", " << i.weight << std::endl;

    return os;
  }
};

enum class FinishConditionType
{
  NUMBER_OF_UPDATES,
  REQUIRED_FITNESS,
  CONVERGENCY_OF_FITNESS,
};

struct FinishCondition
{
  FinishConditionType finish_condition_type;
  int number_of_updates;
  double required_fitness;
  int convergence_accuracy;

  FinishCondition()
      : finish_condition_type(FinishConditionType::NUMBER_OF_UPDATES),
        number_of_updates(1), required_fitness(0), convergence_accuracy(0)
  {
  }

  FinishCondition(const FinishCondition &other)
      : finish_condition_type(other.finish_condition_type),
        number_of_updates(other.number_of_updates),
        required_fitness(other.required_fitness),
        convergence_accuracy(other.convergence_accuracy)
  {
  }

  bool operator==(const FinishCondition &other) const
  {
    return finish_condition_type == other.finish_condition_type &&
           number_of_updates == other.number_of_updates &&
           required_fitness == other.required_fitness &&
           convergence_accuracy == other.convergence_accuracy;
  };

  friend std::ostream &operator<<(std::ostream &os, const FinishCondition &fc)
  {
    std::cout << "number_of_updates " << fc.number_of_updates << std::endl
              << "required_fitness " << fc.required_fitness << std::endl
              << "convergence_accuracy " << fc.convergence_accuracy
              << std::endl;
    return os;
  }
};

struct AlgorithmState
{
  int number_of_updates;
  double previous_fitness;
  int convergence_counter;

  AlgorithmState()
      : number_of_updates(0), previous_fitness(0), convergence_counter(1)
  {
  }

  friend std::ostream &operator<<(std::ostream &os, const AlgorithmState &as)
  {
    std::cout << "number_of_updates " << as.number_of_updates << std::endl
              << "previous_fitness " << as.previous_fitness << std::endl
              << "convergence_counter " << as.convergence_counter << std::endl;
    return os;
  }
};

} // namespace GeneticAlgorithmTypes