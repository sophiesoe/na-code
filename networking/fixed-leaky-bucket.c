#include <stdio.h>
#include <time.h>
#include <unistd.h>

// Global variables for leaky bucket
int bucket_capacity = 20; // Maximum bucket capacity
int current_level = 0;    // Current water level in bucket
int leak_rate = 3;        // Rate at which bucket leaks (packets per second)
time_t last_leak_time;    // Last time the bucket leaked

// Initialize the leaky bucket
void initialize_bucket(int capacity, int rate)
{
  bucket_capacity = capacity;
  current_level = 0;
  leak_rate = rate;
  last_leak_time = time(NULL);

  printf("Leaky Bucket initialized:\n");
  printf("- Capacity: %d packets\n", capacity);
  printf("- Leak Rate: %d packets/second\n", rate);
  printf("- Initial Level: %d packets\n\n", current_level);
}

// Simulate the leaking process
void leak_bucket()
{
  time_t current_time = time(NULL);
  int time_elapsed = current_time - last_leak_time;

  if (time_elapsed > 0)
  {
    // Calculate how many packets should leak out
    int packets_to_leak = time_elapsed * leak_rate;

    if (packets_to_leak > current_level)
    {
      packets_to_leak = current_level;
    }

    current_level -= packets_to_leak;
    last_leak_time = current_time;

    if (packets_to_leak > 0)
    {
      printf("Leaked %d packets. Current level: %d/%d\n",
             packets_to_leak, current_level, bucket_capacity);
    }
  }
}

// Add a packet to the bucket
int add_packet(int packet_size)
{
  // First, simulate leaking
  leak_bucket();

  printf("Attempting to add packet of size %d...\n", packet_size);

  // Check if packet can fit in bucket
  if (current_level + packet_size <= bucket_capacity)
  {
    current_level += packet_size;
    printf("✓ Packet accepted. Current level: %d/%d\n",
           current_level, bucket_capacity);
    return 1; // Success
  }
  else
  {
    printf("✗ Packet dropped! Bucket overflow. Current level: %d/%d\n",
           current_level, bucket_capacity);
    return 0; // Failure - packet dropped
  }
}

// Display current bucket status
void print_status()
{
  leak_bucket();
  float percentage = (float)current_level / bucket_capacity * 100;
  printf("Bucket Status: %d/%d packets (%.1f%% full)\n",
         current_level, bucket_capacity, percentage);
}

// Simple traffic simulation without arrays
void simulate_basic_traffic()
{
  printf("=== Basic Traffic Simulation ===\n");

  int accepted = 0, dropped = 0, total = 0;
  int packet_size;

  // Simulate 10 packets with different sizes
  printf("\n--- Packet 1 (size 5) ---\n");
  packet_size = 5;
  if (add_packet(packet_size))
    accepted++;
  else
    dropped++;
  total++;
  print_status();
  sleep(1);

  printf("\n--- Packet 2 (size 3) ---\n");
  packet_size = 3;
  if (add_packet(packet_size))
    accepted++;
  else
    dropped++;
  total++;
  print_status();
  sleep(1);

  printf("\n--- Packet 3 (size 8) ---\n");
  packet_size = 8;
  if (add_packet(packet_size))
    accepted++;
  else
    dropped++;
  total++;
  print_status();
  sleep(1);

  printf("\n--- Packet 4 (size 12) ---\n");
  packet_size = 12;
  if (add_packet(packet_size))
    accepted++;
  else
    dropped++;
  total++;
  print_status();
  sleep(1);

  printf("\n--- Packet 5 (size 7) ---\n");
  packet_size = 7;
  if (add_packet(packet_size))
    accepted++;
  else
    dropped++;
  total++;
  print_status();
  sleep(1);

  printf("\n=== Simulation Results ===\n");
  printf("Total Packets: %d\n", total);
  printf("Packets Accepted: %d\n", accepted);
  printf("Packets Dropped: %d\n", dropped);
  printf("Drop Rate: %.2f%%\n", (float)dropped / total * 100);
}

// Interactive simulation
void interactive_simulation()
{
  printf("\n=== Interactive Mode ===\n");
  printf("Enter packet sizes (0 to exit):\n");

  int packet_size;
  int accepted = 0, dropped = 0, total = 0;

  while (1)
  {
    printf("\nEnter packet size: ");
    scanf("%d", &packet_size);

    if (packet_size <= 0)
    {
      break;
    }

    printf("\n--- Processing packet of size %d ---\n", packet_size);

    if (add_packet(packet_size))
    {
      accepted++;
    }
    else
    {
      dropped++;
    }
    total++;

    print_status();

    // Simulate 1 second processing time
    sleep(1);
  }

  if (total > 0)
  {
    printf("\n=== Interactive Session Results ===\n");
    printf("Total Packets: %d\n", total);
    printf("Packets Accepted: %d\n", accepted);
    printf("Packets Dropped: %d\n", dropped);
    printf("Drop Rate: %.2f%%\n", (float)dropped / total * 100);
  }
}

// Burst traffic test
void test_burst_traffic()
{
  printf("\n=== Burst Traffic Test ===\n");
  printf("Sending rapid burst of packets...\n");

  int i;
  int accepted = 0, dropped = 0;

  // Send 5 large packets quickly (burst)
  for (i = 1; i <= 5; i++)
  {
    printf("\n--- Burst packet %d (size 10) ---\n", i);

    if (add_packet(10))
    {
      accepted++;
    }
    else
    {
      dropped++;
    }

    print_status();
    // Very short delay to simulate burst
    usleep(500000); // 0.5 seconds
  }

  printf("\n=== Burst Test Results ===\n");
  printf("Burst Packets Sent: 5\n");
  printf("Packets Accepted: %d\n", accepted);
  printf("Packets Dropped: %d\n", dropped);

  // Wait and show recovery
  printf("\nWaiting 3 seconds for bucket to leak...\n");
  sleep(3);
  print_status();
}

// Rate limiting demonstration
void demonstrate_rate_limiting()
{
  printf("\n=== Rate Limiting Demonstration ===\n");

  // Reset bucket
  current_level = 0;
  last_leak_time = time(NULL);

  printf("Sending packets at different rates...\n");

  int i;

  // Send packets slowly (within rate limit)
  printf("\n-- Slow sending (2 second intervals) --\n");
  for (i = 1; i <= 3; i++)
  {
    printf("Slow packet %d:\n", i);
    add_packet(5);
    print_status();
    sleep(2); // 2 second delay
  }

  // Send packets quickly (exceeding rate limit)
  printf("\n-- Fast sending (0.5 second intervals) --\n");
  for (i = 1; i <= 3; i++)
  {
    printf("Fast packet %d:\n", i);
    add_packet(8);
    print_status();
    usleep(500000); // 0.5 second delay
  }
}

int main()
{
  printf("=== Leaky Bucket Algorithm (Simple Version) ===\n\n");

  // Initialize bucket with capacity 20 and leak rate 3 packets/second
  initialize_bucket(20, 3);

  int choice;

  printf("Choose simulation mode:\n");
  printf("1. Basic automatic simulation\n");
  printf("2. Interactive mode\n");
  printf("3. Burst traffic test\n");
  printf("4. Rate limiting demonstration\n");
  printf("5. Run all tests\n");
  printf("Enter choice (1-5): ");
  scanf("%d", &choice);

  switch (choice)
  {
  case 1:
    simulate_basic_traffic();
    break;

  case 2:
    interactive_simulation();
    break;

  case 3:
    test_burst_traffic();
    break;

  case 4:
    demonstrate_rate_limiting();
    break;

  case 5:
    simulate_basic_traffic();
    test_burst_traffic();
    demonstrate_rate_limiting();
    break;

  default:
    printf("Invalid choice. Running basic simulation...\n");
    simulate_basic_traffic();
  }

  printf("\n=== Program finished ===\n");
  return 0;
}