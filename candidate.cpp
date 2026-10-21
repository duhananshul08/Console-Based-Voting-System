#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "candidate.h"
#include "utils.h"

std::vector<Candidate> load_candidates()
{
    std::vector<Candidate> candidates;
    std::ifstream file(CANDIDATE_FILE);
    std::string line;

    while (static_cast<int>(candidates.size()) < MAX_CANDIDATES && std::getline(file, line))
    {
        // format: id|name|party
        std::size_t p1 = line.find('|');
        if (p1 == std::string::npos)
            continue;

        std::size_t p2 = line.find('|', p1 + 1);
        if (p2 == std::string::npos)
            continue;

        Candidate c;

        try
        {
            c.id = std::stoi(line.substr(0, p1));
        }
        catch (...)
        {
            continue;
        }

        c.name  = line.substr(p1 + 1, p2 - p1 - 1);
        c.party = line.substr(p2 + 1);

        if (!c.name.empty() && !c.party.empty())
            candidates.push_back(c);
    }

    return candidates;
}

int add_candidate(const std::string& name, const std::string& party)
{
    std::vector<Candidate> candidates = load_candidates();
    int next_id = 1;

    for (const Candidate& c : candidates)
    {
        if (c.name == name && c.party == party)
            return -1;

        if (c.id >= next_id)
            next_id = c.id + 1;
    }

    if (static_cast<int>(candidates.size()) >= MAX_CANDIDATES)
        return -2;

    std::ofstream file(CANDIDATE_FILE, std::ios::app);

    if (!file)
        return -2;

    file << next_id << '|' << name << '|' << party << '\n';

    return next_id;
}

void list_candidates()
{
    std::vector<Candidate> candidates = load_candidates();

    if (candidates.empty())
    {
        std::cout << "No candidates registered.\n";
        return;
    }

    std::cout << "\nRegistered Candidates:\n";

    for (const Candidate& c : candidates)
        std::cout << c.id << ". " << c.name << " (" << c.party << ")\n";
}

void candidate_registration(int duration)
{
    std::time_t end_time = std::time(nullptr) + duration;

    std::cout << "\n=== CANDIDATE REGISTRATION ===\n";

    while (time_left(end_time) > 0)
    {
        std::cout << "\nTime left: " << time_left(end_time) << " seconds\n";
        std::cout << "Enter command (add/list/end): ";
        std::string command = get_input();

        if (command == "add")
        {
            std::cout << "Candidate name: ";
            std::string name = get_input();

            std::cout << "Party name: ";
            std::string party = get_input();

            clean_field(name);
            clean_field(party);

            if (name.empty() || party.empty())
            {
                std::cout << "Name and party are required.\n";
                continue;
            }

            int id = add_candidate(name, party);

            if (id == -1)
                std::cout << "Candidate already exists.\n";
            else if (id == -2)
                std::cout << "Could not add candidate (file error or limit reached).\n";
            else
                std::cout << "Candidate added with ID " << id << ".\n";
        }
        else if (command == "list")
        {
            list_candidates();
        }
        else if (command == "end")
        {
            std::cout << "Enter admin password: ";
            std::string password = get_input();

            if (password == ADMIN_PASSWORD)
            {
                std::cout << "Registration ended.\n";
                break;
            }

            std::cout << "Incorrect password.\n";
        }
        else
        {
            std::cout << "Invalid command.\n";
        }
    }

    if (time_left(end_time) == 0)
        std::cout << "\nCandidate registration time expired.\n";
}
