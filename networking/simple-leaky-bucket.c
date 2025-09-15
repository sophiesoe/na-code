#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_QUEUE_SIZE 20
#define BUCKET_SIZE 10

// Simple packet structure
typedef struct
{
  int size;
  int id;
} Packet;

// Simple queue for packets
Packet queue[MAX_QUEUE_SIZE];
int queue_front = 0;
int queue_rear = 0;
int queue_count = 0;

// Add packet to queue
void enqueue_packet(int size, int id)
{
  if (queue_count < MAX_QUEUE_SIZE)
  {
    queue[queue_rear].size = size;
    queue[queue_rear].id = id;
    queue_rear = (queue_rear + 1) % MAX_QUEUE_SIZE;
    queue_count++;
    printf("Added packet %d (size %d) to queue no %d\n", id, size, queue_count);
  }
  else
  {
    printf("Queue full! Packet %d (size %d) dropped\n", id, size);
  }
}

// Remove packet from front of queue
Packet dequeue_packet()
{
  Packet p;
  if (queue_count > 0)
  {
    p = queue[queue_front];
    queue_front = (queue_front + 1) % MAX_QUEUE_SIZE;
    queue_count--;
    return p;
  }
  p.size = -1; // Invalid packet indicator
  p.id = -1;
  return p;
}

// Check packet at head of queue without removing
Packet peek_packet()
{
  Packet p;
  if (queue_count > 0)
  {
    p = queue[queue_front];
    return p;
  }
  p.size = -1; // Invalid packet indicator
  p.id = -1;
  return p;
}

// Send packet into network (simulation)
void send_packet(Packet p)
{
  printf("SENT: Packet %d (size %d) into the network\n", p.id, p.size);
}

// Display current queue status
void show_queue_status()
{
  printf("Queue status: %d packets waiting\n", queue_count);
  if (queue_count > 0)
  {
    printf("Next packet: ID=%d, size=%d\n",
           queue[queue_front].id, queue[queue_front].size);
  }
}

// Main leaky bucket algorithm - following exact steps
void leaky_bucket_algorithm()
{
  int counter;
  int tick = 1;

  printf("\n=== Starting Leaky Bucket Algorithm ===\n");
  printf("Bucket size (n): %d\n", BUCKET_SIZE);

  while (queue_count > 0)
  { // Continue while packets exist
    printf("\n--- CLOCK TICK %d ---\n", tick);

    // Initialize counter to n at the tick of the clock
    counter = BUCKET_SIZE;
    printf("Step: Initialize counter to n = %d\n", counter);

    // Step 1: Repeat until n is smaller than packet size at head of queue
    while (1)
    {
      // Check if queue is empty
      if (queue_count == 0)
      {
        printf("Queue is empty - no more packets to process\n");
        break;
      }

      // Check packet at head of queue
      Packet head_packet = peek_packet();

      printf("Counter = %d, Head packet size = %d\n", counter, head_packet.size);

      // Check condition: is counter smaller than packet size?
      if (counter < head_packet.size)
      {
        printf("Counter (%d) < Packet size (%d) - stopping this tick\n",
               counter, head_packet.size);
        break; // Exit the repeat loop
      }

      // Step 1.1: Pop a packet out of the head of the queue
      Packet p = dequeue_packet();
      printf("Step 1.1: Popped packet %d (size %d) from queue\n", p.id, p.size);

      // Step 1.2: Send the packet into the network
      printf("Step 1.2: ");
      send_packet(p);

      // Step 1.3: Decrement the counter by the size of packet
      counter = counter - p.size;
      printf("Step 1.3: Decremented counter by %d, new counter = %d\n",
             p.size, counter);

      printf("\n\nRemaining packets in queue: %d\n", queue_count);
    }

    // Step 2: Reset counter and go to step 1 (next clock tick)
    printf("Step 2: Reset counter and wait for next clock tick\n");

    // Show status before next tick
    show_queue_status();

    tick++;

    // Simulate clock tick delay
    sleep(2);
  }

  printf("\n=== Complete - All packets processed ===\n");
}

int main()
{
  enqueue_packet(3, 101); // Packet ID 101, size 3
  enqueue_packet(2, 102); // Packet ID 102, size 2
  enqueue_packet(5, 103); // Packet ID 103, size 5
  enqueue_packet(4, 104); // Packet ID 104, size 4
  enqueue_packet(1, 105); // Packet ID 105, size 1
  enqueue_packet(6, 106); // Packet ID 106, size 6
  enqueue_packet(3, 107); // Packet ID 107, size 3
  enqueue_packet(7, 108); // Packet ID 108, size 7

  printf("\nInitial ");
  show_queue_status();

  // Run the leaky bucket algorithm
  leaky_bucket_algorithm();

  return 0;
}