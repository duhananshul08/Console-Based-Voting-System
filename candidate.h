#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
#include <vector>
#include "config.h"

std::vector<Candidate> load_candidates();
int  add_candidate(const std::string& name, const std::string& party); // id, -1 duplicate, -2 error/limit
void list_candidates();
void candidate_registration(int duration);

#endif
