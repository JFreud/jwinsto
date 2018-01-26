# Fencing Tournament Management
## Jerome Freudenberg, Max Chan (Period 4)
---
## Our Project:

The current system the United States Fencing Association employs to transfer and display results at tournaments is outdated and obsolete. Referees from across the hall must send the fencers to turn in their results at the end of a bout or pool, which are then manually entered into the system. This renders the process slow and prone to human error, something that could be avoided by digitizing the process and having referees submit results directly, thereby reducing wait-time and allowing fencers to immediately catch mistakes in scoring. The goal is to minimize the role of the so-called “bout committee” and ultimately replace it completely.  This is accomplished by creating a committee server, which referees interact with directly through sockets. After each pool or bout, a referee will enter the data, which the committee processes, and then displays, streamlining the whole processing and making it more efficient.

This was meant to be easy, but clearly isn't.  Enjoy!

## How to use our project

1) In the director, run `make`
2) On one shell, execute the Bout Committee entering `./committee`
3) On a seperate shell, take the role of a referee by entering `./referee`.  Note, that you must be a registered referee on the ref list to access this.  (Referees for this tournament are aziz, nazarova, chan)
4) Depending on status, enter either 1 or 2 for pools or DEs, respectively.
5) You, as the referee, will be prompted to enter data for the bouts you reffed.  For convenience, change `debug` (in bout_commitee.c, line 21) to 1 to have pre-set data entered for you
6) After Pools, you will receive your DE assignments, and be once again prompted to order them.  This continues until you are out of assignments, which signals the committee to give you new bouts, from the new tableau.

## Bugs and Issues
* Occasionally seg faults ` ¯\_(ツ)_/¯ `
* Occasionally reads in questionable fencer
* Very not robust
  * Referees must be very careful typing as putting a typo in will result in the results not going through
  * If two fencers have the same last name, the second seeded one will probably disappear off the face of the earth
  * etc
* Not very user friendly
* Doesn't account for different fencing tournament forms (e.g. NACs cut off the bottom 20% before calculating DE bouts)
* (NOTE: RESOLVED) DEs won't work for any event greater than 2^12 fencers. (Which hasn't happened yet but that's what they thought with IPs)
* Assumes referee user has common sense, and is not trying to break code.  Often, a naive assumption
