# Philosophers

## General notes

- Compile and link with `-pthread`
- Protect your threads!

## Solving issues ideas

- [ ] Making a counter for the number of times the philo tried to take the forks and prioritizing those who tried more
- [ ] Making a waiting condition involved the ratio time_since_eaten/time_to_die (to combine with above)
- [ ] Making philos that just slept wait for time_to_eat
- [ ] Making already fed philos wait for time_to_eat