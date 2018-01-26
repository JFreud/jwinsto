# Fencing Tournament Management
## Jerome Freudenberg, Max Chan (Period 4)
---
## Our Project:

The current system the United States Fencing Association employs to transfer and display results at tournaments is outdated and obsolete. Referees from across the hall must send the fencers to turn in their results at the end of a bout or pool, which are then manually entered into the system. This renders the process slow and prone to human error, something that could be avoided by digitizing the process and having referees submit results directly, thereby reducing wait-time and allowing fencers to immediately catch mistakes in scoring. The goal is to minimize the role of the so-called “bout committee” and ultimately replace it completely.  This is accomplished by creating a committee server, which referees interact with directly through sockets. After each pool or bout, a referee will enter the data, which the committee processes, and then displays, streamlining the whole processing and making it more efficient.

This was meant to be easy, but clearly isn't.  Enjoy!

## How to use our project

### PLEASE READ THIS CAREFULLY.  ANY DEVIATION FROM THESE INSTRUCTIONS WILL HAVE DISASTAROUS CONSEQUENCES, ESPECIALLY TO OUR GRADES

1) After opening a terminal shell, navigate to the ./jwinsto and run `make`
2) Execute the Bout Committee by entering `./committee`
3) Opening a new terminal shell in the same directory, take the role of a referee by entering `./referee`. You will be prompted to enter either 1 or 2 for pools or DEs, respectively, then to identify yourself.  Note, that you must be a registered referee on the ref list to access this.  (Referees for this tournament are aziz, nazarova, chan)

In a normal tournament format, pools would be fenced first, then DEs.  However, for the convenience of the reviewer, `easy_pool` and `easy_DE` (in bout_commitee.c, line 21 and 22) are set to 1, which automatically enters pre-determined pool results, as well as for the first round of DEs (The table of 16). If you choose to use these pre-set results, skip step 4.  

4) To manually enter pools or first round DE results, open bout_committee.c, and set `easy_pool` and `easy_DE` to 0, then repeat steps 1-3 above.  You will then be prompted to enter (in succession): Bout Winner, Winner Score, Bout Loser, Loser Score.  Identify the winner and loser by their last name in lower-case, and use numbers to report their score.  If you are unsure of which fencers you are reffing, the bout_committee prints the fencers in your pool.

    Each pool is led by one ref, which anywhere from 5-9 fencers in each.  Pools are fenced in a round-robin style, where each fencer fences everyone in the pool once, to five touches.  It is assumed the referee can ref, and accurately executes and directs the bout, such that this happens.  In testing this code, take extra caution that all names are spelled correctly (typos are not to be forgiven), that every fencer has fenced everyone, and that no one has fenced anyone twice.

* NOTE: Due to a bug we haven't had a chance to fix, after all pool results have been entered, you must hit any letter, and then hit enter to continue.  

    The tournament cannot continue until all pool results from every referee are entered. Therefore, after entering data for one referee, exit the `./referee` program, and run it again, but this time for a different referee.  Repeat this step (step 4) until pool results for all referees (aziz, nazarova, chan) have been entered.

5) After Pools, you will receive your DE assignments, and be once again prompted to enter results for them.  Post pool seeding is determined by results in pools.  First, the Win/Loss ratio is considered, and if people have the same fraction, the Indicator (Number of Touches scored, minus number of touches received) is used to order the fencers.

    DE bouts match the strongest fencer with the weakest fencer.  For example, if 16 people were in a tournament, the fencer seeded 1st after pools would fence the fencer seeded 16th, the fencer seeded 2nd would fence the fencer seeded 15th, and so on.  If the number of fencers is not an exponent of 2, a number of fencers receive a BYE, meaning a direct pass to the next round of DEs.  The remaining fencers compete to fill the slots in that Tableau, and therefore advance in the tournament

    To enter DE pools, first repeat step 3, but this time hitting 2 for DEs.  Again, you must identify yourself with the last name of a registered referee, after which your assignments will be visible in the bout committee page.

    Similarly to pools, you will be prompted to enter Bout Winner, Winner Score, Bout Loser, and Loser Score.  The winner and loser are once again identified by their last name, all lower-case, and score are in numbers.

    DE bouts are fenced to 15 touches, but are elimination style.  When a fencer loses, they are removed from the tournament.  Therefore it is necessary, as with pools, that all referees finish their first round of DE assignments before the tournament can progress.  Repeat Step 5 for all referees (aziz, nazarova, chan).

6) After the first round of DEs have been fenced, the number of fencers halves.  Repeat Step 5, UNTIL THERE ARE FOUR FENCERS LEFT (identifiable by the left columb of the tree like structure displaying: `Tableau: 4`)

    At this point in time, it is the semi-finals (hence four fencers).  THIS MEANS ONLY TWO REFEREES ARE NEEDED FOR THIS ROUND.  Repeat step 5 for Refeees aziz and nazarova ONLY. DO NOT enter data for referee chan, as he is not needed to ref this round of DES.

    On the next round (Finals), ONLY ONE REFEREE IS NEEDED.  ONLY enter data for referee aziz.  Referees nazarova and chan ARE NOT NEEDED.

    Upon completion of the final, the winner of the tournament is displayed.

## Bugs and Issues
* Occasionally seg faults ` ¯\_(ツ)_/¯ `
* Occasionally reads in questionable fencer
* Very not robust
  * Referees must be very careful typing as putting a typo in will result in the results not going through
  * If two fencers have the same last name, the second seeded one will probably disappear off the face of the earth
  * etc
* Not very user friendly (not friendly to coders either :( )
* Doesn't account for different fencing tournament forms (e.g. NACs cut off the bottom 20% before calculating DE bouts)
* (NOTE: RESOLVED) DEs won't work for any event greater than 2^12 fencers. (Which hasn't happened yet but that's what they thought with IPs)
* Assumes referee user has common sense, and is not trying to break code.  Often, a naive assumption
