# 1ο Εργαστήριο Αρχιτεκτονικής Υπολογιστών

1) Ελέγχοντας το αρχείο [starter_se.py](https://github.com/uart/gem5-mirror/blob/master/configs/example/arm/starter_se.py) παρατηρούμε ότι ο default τύπος cpu αν δεν δώσουμε όρισμα είναι ο _atomic_

```python
parser.add_argument("--cpu", type=str, choices=list(cpu_types.keys()),
                        default="atomic",
                        help="CPU model to use")
```

η συχνότητα συστήματος είναι 1GHz (δεν μπορούμε να την αλλάξουμε με κάποιο όρισμα μόνο αλλάζοντας το starter_se.py αρχείο).

```python
self.clk_domain = SrcClockDomain(clock="1GHz",
                                         voltage_domain=self.voltage_domain)
```

Επιπλέον μέσω ορισμάτων μπορούμε να αλλάξουμε τη συχνότητα του επεξεργαστή (default 4GHz), τον αριθμό πυρήνων(default 4 πυρήνες), τον τύπο και την ταχύτητα της μνήμης RAM (default DDR3 1600Hz) και το μέγεθος της μνήμης (default 2GB).

```python
parser.add_argument("--cpu-freq", type=str, default="4GHz")
parser.add_argument("--num-cores", type=int, default=1,
                        help="Number of CPU cores")
parser.add_argument("--mem-type", default="DDR3_1600_8x8",
                        choices=ObjectList.mem_list.get_names(),
                        help = "type of memory to use")
parser.add_argument("--mem-channels", type=int, default=2,
                        help = "number of memory channels")
parser.add_argument("--mem-size", action="store", type=str,
                        default="2GB",
                        help="Specify the physical memory size")
```

Τέλος βλέπουμε ότι έχουμε L1 και L2 cache 

```python
if self.cpu_cluster.memoryMode() == "timing":
    self.cpu_cluster.addL1()
    self.cpu_cluster.addL2(self.cpu_cluster.clk_domain)
```

2) Διαβάζοντας το stats.txt και την επεξήγηση κάθε στατιστικού βλέπουμε οτι:
    * sim_seconds: Ο συνολικός χρόνος της προσομοίωσης σε δευτερόλεπτα
    * sim_insts: Ο αριθμός των εντολών που εκτελέι η CPU
    * host_inst_rate: Δείκτης απόδοσης του συστήματος, υπολογίζεται ως εντολές/δευτερόλεπτα

3) Απο την εκφώνηση ξέρουμε ότι (miss penalty) L1 = 6 cycles (miss penalty), L2 = 50 cycles και 1 cycle cache hit/instruction execution. Επίσης απο το αρχείο stats.txt μετά το simulation βλέπουμε ότι:

```text
system.cpu_cluster.cpus.dcache.overallMisses::total          179 

system.cpu_cluster.cpus.icache.overallMisses::total          332

system.cpu_cluster.l2.overallMisses::total                   479 

simInsts                                                     5028
```

Επομένως ```IL1.miss_num = 332, DL1.miss_num = 179,L2.miss_num = 479, Total_Inst_num = 5028``` και χρησιμοποιώντας την εξίσωση της εκφώνησης ![Formula](https://user-images.githubusercontent.com/43075884/143724729-b66cb8b2-3c21-4102-a2a6-818282ddf972.png)  
προκύπτει ότι  
![New eq](https://user-images.githubusercontent.com/43075884/143724745-9f366519-f747-4359-9e0c-b892b0535343.png)  
και τελικά  ```CPI = 6.37```

4) Υπάρχουν δύο τύποι in-order CPU μοντέλων [[1]](#Πηγές) [[2]](#Πηγές) [[3]](#Πηγές)
    * **SimpleCPU**: Simple abstract CPU without a pipeline. They are therefore completely unrealistic. But they also run much faster.
        * **AtomicSimpleCPU**: The AtomicSimpleCPU is the version of SimpleCPU that uses atomic memory accesses. Memory accesses happen instantaneously. The fastest simulation except for KVM, but not realistic at all. It uses the latency estimates from the atomic accesses to estimate overall cache access time. The AtomicSimpleCPU is derived from BaseSimpleCPU, and implements functions to read and write memory, and also to tick, which defines what happens every CPU cycle. It defines the port that is used to hook up to memory, and connects the CPU to the cache.
        * **TimingSimpleCPU**: The TimingSimpleCPU is the version of SimpleCPU that uses timing memory accesses. Memory accesses are realistic, but the CPU has no pipeline. The simulation is faster than detailed models, but slower than AtomicSimpleCPU. It stalls on cache accesses and waits for the memory system to respond prior to proceeding. Like the AtomicSimpleCPU, the TimingSimpleCPU is also derived from BaseSimpleCPU, and implements the same set of functions. It defines the port that is used to hook up to memory, and connects the CPU to the cache. It also defines the necessary functions for handling the response from memory to the accesses sent out.  
    * **MinorCPU**: Generic in-order superscalar core. Minor is an in-order processor model with a fixed pipeline but configurable data structures and execute behaviour. It is intended to be used to model processors with strict in-order execution behaviour and allows visualisation of an instruction’s position in the pipeline through the MinorTrace/minorview.py format/tool. The intention is to provide a framework for micro-architecturally correlating the model with a particular, chosen processor with similar capabilities. Its C++ implementation that can be parametrized to more closely match real cores. The following models extend the MinorCPU class by parametrization to make it match existing CPUs more closely:
        * **HPI**: The HPI CPU timing model is tuned to be representative of a modern in-order Armv8-A implementation.
        * **ex5_LITTLE**: ex5 LITTLE core (based on the ARM Cortex-A7)

Τρέχοντας το ίδιο πρόγραμμα (20x20MatrixMult) με τις ίδιες παραμέτρους (--caches) στους 2 επεξεργαστές έχουμε:  
Για τον TimingSimpleCPU: ```simSeconds                                   0.001714```  
Για τον MinorCPU: ```simSeconds                                   0.000895```

Δοκιμάζουμε τώρα για τους 2 επεξεργαστές τους συνδιασμούς μνήμης  LPDDR2_S4_1066_1x32, DDR4_2400_4x16 και συχνότητα CPU 1GHz και 4GHz  

|                 | 1GHz/DDR4_2400_4x16 | 4GHz/DDR4_2400_4x16 | 1GHz/LPDDR2_S4_1066_1x32 | 4GHz/LPDDR2_S4_1066_1x32 |
|:---------------:|---------------------|---------------------|--------------------------|--------------------------|
| TimingSimpleCPU |       0.003396      |       0.000875      |         0.003405         |         0.000885         |
| MinorCPU        |       0.001576      |       0.000554      |         0.001608         |         0.000595         |  

Παρατηρούμε ότι ο MinorCPU έχει λίγο μεγαλύτερη ευαισθησία στην αλλαγή τύπου μνήμης από τον TimingSimpleCPU. Παρ' ότι οι προσβάσεις μνήμεις του TimingSimpleCPU είναι ρεαλιστικές ίσως η ύπαρξη του pipeline να δημιουργεί μεγαλύτερη ευαισθησία στην αλλαγή τύπου μνήμης στον MinorCPU.
  
Ως προς την συχνότητα CPU βλέπουμε ότι ο TimingSimpleCPU έχει μεγαλύτερη ευαισθησία απο τον MinorCPU (με την αλλαγή απο 1GHz σε 4GHz ο χρόνος του TimingSimpleCPU μειώνεται περίπου 4 φορές ενώ του MinorCPU 3). Η διαφορά αυτή ξανά ίσως οφείλεται στην έλλειψη pipeline του TimingSimpleCPU

# Πηγές
[1\] [https://cirosantilli.com/linux-kernel-module-cheat/#gem5-basesimplecpu](https://cirosantilli.com/linux-kernel-module-cheat/#gem5-basesimplecpu)  
[2\] [https://www.gem5.org/documentation/general_docs/cpu_models/SimpleCPU](https://www.gem5.org/documentation/general_docs/cpu_models/SimpleCPU)  
[3\] [https://www.gem5.org/documentation/general_docs/cpu_models/minor_cpu](https://www.gem5.org/documentation/general_docs/cpu_models/minor_cpu)
