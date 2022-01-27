#include <systemc.h>

#include "ga_module.hh"


int sc_main(int argc, char *argv[])
{
  sc_signal<int> number_of_items;
  number_of_items.write(4);

  std::vector<int> values({1, 2, 1, 3});
  std::vector<int> weights({2, 1, 4, 5}); // 0.5+2+0.6=3.1
  std::vector<Item> items;
  for (int i = 0; i < values.size(); i++)
    items.emplace_back(values[i], weights[i]);

  sc_signal<double> p_co, p_mu;
  p_co.write(0.2);
  p_mu.write(0.1);

  sc_signal<int> size_of_population;
  size_of_population.write(5);

  sc_signal<int> weight_limit;
  weight_limit.write(8);

  FinishCondition finish_condition;
  finish_condition.finish_condition_type =
  FinishConditionType::CONVERGENCY_OF_FITNESS;
  finish_condition.convergence_accuracy = 5;


  GeneticAlgorithm ga("genetic algo");
  ga.size_of_population(size_of_population);
  ga.number_of_items(number_of_items);
  ga.items = items;
  ga.p_cross_over(p_co);
  ga.p_mutation(p_mu);
  ga.weight_limit(weight_limit);
  ga.finish_condition = finish_condition;
  auto &answer = ga.answer;

  //   sc_trace_file *fp;
  //   fp = sc_create_vcd_trace_file("vcd_trace");
  //   fp->set_time_unit(1, SC_MS);

  //   sc_trace(fp, train_arrival, "INPUT");
  //   sc_trace(fp, cars_road_state, "OUTPUT");

  sc_start(15, SC_SEC);

  std::cout << answer << std::endl;
  // sc_close_vcd_trace_file(fp);

  return 0;
}
