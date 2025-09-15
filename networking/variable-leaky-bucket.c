#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

// Global variables for variable leak bucket
int bucket_capacity = 30;
int current_level = 0;
int base_leak_rate = 3;    // Base leak rate (packets/second)
int current_leak_rate = 3; // Current dynamic leak rate
time_t last_leak_time;
int leak_mode = 1; // 1=adaptive, 2=scheduled, 3=load-based, 4=priority

// Statistics for monitoring
int total_packets_received = 0;
int total_packets_accepted = 0;
int total_packets_dropped = 0;
int leak_rate_changes = 0;

// Initialize the variable leak bucket
void initialize_variable_bucket(int capacity, int base_rate)
{
  bucket_capacity = capacity;
  current_level = 0;
  base_leak_rate = base_rate;
  current_leak_rate = base_rate;
  last_leak_time = time(NULL);

  printf("Variable Leak Bucket initialized:\n");
  printf("- Capacity: %d packets\n", capacity);
  printf("- Base Leak Rate: %d packets/second\n", base_rate);
  printf("- Current Leak Rate: %d packets/second\n", current_leak_rate);
  printf("- Initial Level: %d packets\n\n", current_level);
}

// Adaptive leak rate based on bucket fill level
void adaptive_leak_rate()
{
  int old_rate = current_leak_rate;
  float fill_percentage = (float)current_level / bucket_capacity;

  if (fill_percentage > 0.8)
  {
    // High fill: increase leak rate significantly
    current_leak_rate = base_leak_rate * 3;
  }
  else if (fill_percentage > 0.6)
  {
    // Medium-high fill: increase leak rate moderately
    current_leak_rate = base_leak_rate * 2;
  }
  else if (fill_percentage > 0.4)
  {
    // Medium fill: slight increase
    current_leak_rate = (int)(base_leak_rate * 1.5);
  }
  else if (fill_percentage < 0.2)
  {
    // Low fill: reduce leak rate to conserve resources
    current_leak_rate = (int)(base_leak_rate * 0.7);
  }
  else
  {
    // Normal fill: use base rate
    current_leak_rate = base_leak_rate;
  }

  if (old_rate != current_leak_rate)
  {
    printf("ADAPTIVE: Leak rate changed from %d to %d (fill: %.1f%%)\n",
           old_rate, current_leak_rate, fill_percentage * 100);
    leak_rate_changes++;
  }
}

// Scheduled leak rate based on time of day simulation
void scheduled_leak_rate()
{
  int old_rate = current_leak_rate;
  time_t current_time = time(NULL);
  int time_slot = (current_time % 60); // Simulate different time periods

  if (time_slot < 15)
  {
    // Peak hours: high leak rate
    current_leak_rate = base_leak_rate * 2;
  }
  else if (time_slot < 30)
  {
    // Business hours: normal rate
    current_leak_rate = base_leak_rate;
  }
  else if (time_slot < 45)
  {
    // Off-peak: reduced rate
    current_leak_rate = (int)(base_leak_rate * 0.6);
  }
  else
  {
    // Maintenance window: very low rate
    current_leak_rate = 1;
  }

  if (old_rate != current_leak_rate)
  {
    printf("SCHEDULED: Leak rate changed from %d to %d (time slot: %d)\n",
           old_rate, current_leak_rate, time_slot);
    leak_rate_changes++;
  }
}

// Load-based leak rate (simulates system load)
void load_based_leak_rate()
{
  int old_rate = current_leak_rate;
  static int system_load = 50; // Simulated system load (0-100%)

  // Simulate changing system load
  system_load += (rand() % 21 - 10); // Random change -10 to +10
  if (system_load < 0)
    system_load = 0;
  if (system_load > 100)
    system_load = 100;

  if (system_load > 80)
  {
    // High load: reduce leak rate
    current_leak_rate = 1;
  }
  else if (system_load > 60)
  {
    // Medium load: slightly reduce
    current_leak_rate = (int)(base_leak_rate * 0.7);
  }
  else if (system_load > 40)
  {
    // Normal load: base rate
    current_leak_rate = base_leak_rate;
  }
  else if (system_load > 20)
  {
    // Low load: increase rate
    current_leak_rate = (int)(base_leak_rate * 1.5);
  }
  else
  {
    // Very low load: maximum rate
    current_leak_rate = base_leak_rate * 2;
  }

  if (old_rate != current_leak_rate)
  {
    printf("LOAD-BASED: Leak rate changed from %d to %d (load: %d%%)\n",
           old_rate, current_leak_rate, system_load);
    leak_rate_changes++;
  }
}

// Priority-based leak rate (different rates for different packet types)
void priority_leak_rate(int packet_priority)
{
  int old_rate = current_leak_rate;

  switch (packet_priority)
  {
  case 1: // High priority (critical packets)
    current_leak_rate = base_leak_rate * 3;
    break;
  case 2: // Medium priority (important packets)
    current_leak_rate = base_leak_rate * 2;
    break;
  case 3: // Normal priority
    current_leak_rate = base_leak_rate;
    break;
  case 4: // Low priority (background traffic)
    current_leak_rate = (int)(base_leak_rate * 0.5);
    break;
  default:
    current_leak_rate = base_leak_rate;
  }

  if (old_rate != current_leak_rate)
  {
    printf("PRIORITY: Leak rate changed from %d to %d (priority: %d)\n",
           old_rate, current_leak_rate, packet_priority);
    leak_rate_changes++;
  }
}

// Update leak rate based on selected mode
void update_leak_rate(int packet_priority)
{
  switch (leak_mode)
  {
  case 1:
    adaptive_leak_rate();
    break;
  case 2:
    scheduled_leak_rate();
    break;
  case 3:
    load_based_leak_rate();
    break;
  case 4:
    priority_leak_rate(packet_priority);
    break;
  default:
    current_leak_rate = base_leak_rate;
  }
}

// Variable leak simulation
void variable_leak_bucket()
{
  time_t current_time = time(NULL);
  int time_elapsed = current_time - last_leak_time;

  if (time_elapsed > 0)
  {
    // Calculate packets to leak with current rate
    int packets_to_leak = time_elapsed * current_leak_rate;

    if (packets_to_leak > current_level)
    {
      packets_to_leak = current_level;
    }

    current_level -= packets_to_leak;
    last_leak_time = current_time;

    if (packets_to_leak > 0)
    {
      printf("Leaked %d packets at rate %d/sec. Level: %d/%d\n",
             packets_to_leak, current_leak_rate, current_level, bucket_capacity);
    }
  }
}

// Add packet with priority support
int add_packet_with_priority(int packet_size, int priority)
{
  total_packets_received++;

  // Update leak rate based on mode and priority
  update_leak_rate(priority);

  // Perform leaking
  variable_leak_bucket();

  printf("Packet arrived: size=%d, priority=%d\n", packet_size, priority);

  // Check if packet can fit
  if (current_level + packet_size <= bucket_capacity)
  {
    current_level += packet_size;
    total_packets_accepted++;
    printf("Packet accepted. Level: %d/%d (%.1f%% full)\n",
           current_level, bucket_capacity,
           (float)current_level / bucket_capacity * 100);
    return 1;
  }
  else
  {
    total_packets_dropped++;
    printf("Packet dropped! Overflow. Level: %d/%d\n",
           current_level, bucket_capacity);
    return 0;
  }
}

// Standard add packet (normal priority)
int add_packet(int packet_size)
{
  return add_packet_with_priority(packet_size, 3);
}

// Display comprehensive status
void print_detailed_status()
{
  variable_leak_bucket();
  float fill_percentage = (float)current_level / bucket_capacity * 100;
  float accept_rate = total_packets_received > 0 ? (float)total_packets_accepted / total_packets_received * 100 : 0;

  printf("\n BUCKET STATUS:\n");
  printf("   Level: %d/%d packets (%.1f%% full)\n",
         current_level, bucket_capacity, fill_percentage);
  printf("   Current Leak Rate: %d packets/sec\n", current_leak_rate);
  printf("   Base Leak Rate: %d packets/sec\n", base_leak_rate);
  printf("   Mode: %s\n",
         leak_mode == 1 ? "Adaptive" : leak_mode == 2 ? "Scheduled"
                                   : leak_mode == 3   ? "Load-based"
                                                      : "Priority-based");

  printf("\n STATISTICS:\n");
  printf("   Total Received: %d packets\n", total_packets_received);
  printf("   Accepted: %d packets (%.1f%%)\n", total_packets_accepted, accept_rate);
  printf("   Dropped: %d packets (%.1f%%)\n", total_packets_dropped,
         100 - accept_rate);
  printf("   Leak Rate Changes: %d\n", leak_rate_changes);
  printf("-------------------------------------------\n");
}

// Test adaptive leak rate
void test_adaptive_mode()
{
  printf("\n=== ADAPTIVE LEAK RATE TEST ===\n");
  leak_mode = 1;
  current_level = 0;

  // Fill bucket gradually and watch leak rate adapt
  int test_packets[] = {5, 8, 6, 10, 4, 12, 3, 7, 9};
  int num_packets = sizeof(test_packets) / sizeof(test_packets[0]);

  for (int i = 0; i < num_packets; i++)
  {
    printf("\n--- Test packet %d ---\n", i + 1);
    add_packet(test_packets[i]);
    print_detailed_status();
    sleep(1);
  }
}

// Test scheduled leak rate
void test_scheduled_mode()
{
  printf("\n=== SCHEDULED LEAK RATE TEST ===\n");
  leak_mode = 2;
  current_level = 0;

  // Send packets across different time slots
  for (int i = 0; i < 8; i++)
  {
    printf("\n--- Time slot test %d ---\n", i + 1);
    add_packet(5);
    print_detailed_status();
    sleep(8); // Move to next time slot
  }
}

// Test priority-based leak rate
void test_priority_mode()
{
  printf("\n=== PRIORITY-BASED LEAK RATE TEST ===\n");
  leak_mode = 4;
  current_level = 0;

  // Test different priority packets
  int priorities[] = {1, 4, 2, 3, 1, 4, 2};
  int sizes[] = {3, 6, 4, 5, 2, 8, 3};
  int num_packets = sizeof(priorities) / sizeof(priorities[0]);

  for (int i = 0; i < num_packets; i++)
  {
    printf("\n--- Priority test %d ---\n", i + 1);
    add_packet_with_priority(sizes[i], priorities[i]);
    print_detailed_status();
    sleep(2);
  }
}

// Interactive mode selection
void run_interactive_mode()
{
  int choice, packet_size, priority;

  while (1)
  {
    printf("\n=== VARIABLE LEAK BUCKET MENU ===\n");
    printf("1. Set Adaptive Mode\n");
    printf("2. Set Scheduled Mode\n");
    printf("3. Set Load-based Mode\n");
    printf("4. Set Priority-based Mode\n");
    printf("5. Add Normal Packet\n");
    printf("6. Add Priority Packet\n");
    printf("7. Show Status\n");
    printf("8. Reset Bucket\n");
    printf("0. Exit\n");
    printf("Choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      leak_mode = 1;
      printf("✓ Switched to Adaptive Mode\n");
      break;

    case 2:
      leak_mode = 2;
      printf("✓ Switched to Scheduled Mode\n");
      break;

    case 3:
      leak_mode = 3;
      printf("✓ Switched to Load-based Mode\n");
      break;

    case 4:
      leak_mode = 4;
      printf("✓ Switched to Priority-based Mode\n");
      break;

    case 5:
      printf("Enter packet size: ");
      scanf("%d", &packet_size);
      add_packet(packet_size);
      break;

    case 6:
      printf("Enter packet size: ");
      scanf("%d", &packet_size);
      printf("Enter priority (1=high, 2=medium, 3=normal, 4=low): ");
      scanf("%d", &priority);
      add_packet_with_priority(packet_size, priority);
      break;

    case 7:
      print_detailed_status();
      break;

    case 8:
      current_level = 0;
      total_packets_received = 0;
      total_packets_accepted = 0;
      total_packets_dropped = 0;
      leak_rate_changes = 0;
      printf("Bucket reset\n");
      break;

    case 0:
      return;

    default:
      printf("Invalid choice!\n");
    }
  }
}

int main()
{
  printf("=== VARIABLE-LENGTH LEAK BUCKET ALGORITHM ===\n");

  srand(time(NULL)); // Initialize random seed
  initialize_variable_bucket(30, 3);

  int choice;
  printf("\nSelect test mode:\n");
  printf("1. Adaptive leak rate test\n");
  printf("2. Scheduled leak rate test\n");
  printf("3. Priority-based leak rate test\n");
  printf("4. Interactive mode\n");
  printf("5. Run all automated tests\n");
  printf("Enter choice (1-5): ");
  scanf("%d", &choice);

  switch (choice)
  {
  case 1:
    test_adaptive_mode();
    break;

  case 2:
    test_scheduled_mode();
    break;

  case 3:
    test_priority_mode();
    break;

  case 4:
    run_interactive_mode();
    break;

  case 5:
    test_adaptive_mode();
    printf("\n==================================================\n");
    test_priority_mode();
    break;

  default:
    printf("Running adaptive mode by default...\n");
    test_adaptive_mode();
  }

  printf("\n=== FINAL STATISTICS ===\n");
  print_detailed_status();
  printf("Program completed.\n");

  return 0;
}