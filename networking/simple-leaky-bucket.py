import time
from collections import deque

MAX_QUEUE_SIZE = 20
BUCKET_SIZE = 10

# Global queue to store packets
packet_queue = deque()

def create_packet(size, packet_id):
    """Create a packet as a dictionary"""
    return {"size": size, "id": packet_id}

def enqueue_packet(size, packet_id):
    """Add packet to queue"""
    if len(packet_queue) < MAX_QUEUE_SIZE:
        packet = create_packet(size, packet_id)
        packet_queue.append(packet)
        print(f"Added packet {packet_id} (size {size}) to queue no {len(packet_queue)}")
    else:
        print(f"Queue full! Packet {packet_id} (size {size}) dropped")

def dequeue_packet():
    """Remove packet from front of queue"""
    if len(packet_queue) > 0:
        packet = packet_queue.popleft()
        return packet
    return None  # Invalid packet indicator

def peek_packet():
    """Check packet at head of queue without removing"""
    if len(packet_queue) > 0:
        return packet_queue[0]
    return None  # Invalid packet indicator

def send_packet(packet):
    """Send packet into network (simulation)"""
    print(f"SENT: Packet {packet['id']} (size {packet['size']}) into the network")

def show_queue_status():
    """Display current queue status"""
    queue_count = len(packet_queue)
    print(f"Queue status: {queue_count} packets waiting")
    if queue_count > 0:
        next_packet = packet_queue[0]
        print(f"Next packet: ID={next_packet['id']}, size={next_packet['size']}")

def leaky_bucket_algorithm():
    """Main leaky bucket algorithm - following exact steps"""
    tick = 1
    
    print("\n=== Starting Leaky Bucket Algorithm ===")
    print(f"Bucket size (n): {BUCKET_SIZE}")
    
    while len(packet_queue) > 0:  # Continue while packets exist
        print(f"\n--- CLOCK TICK {tick} ---")
        
        # Initialize counter to n at the tick of the clock
        counter = BUCKET_SIZE
        print(f"Step: Initialize counter to n = {counter}")
        
        # Step 1: Repeat until n is smaller than packet size at head of queue
        while True:
            # Check if queue is empty
            if len(packet_queue) == 0:
                print("Queue is empty - no more packets to process")
                break
            
            # Check packet at head of queue
            head_packet = peek_packet()
            
            print(f"Counter = {counter}, Head packet size = {head_packet['size']}")
            
            # Check condition: is counter smaller than packet size?
            if counter < head_packet['size']:
                print(f"Counter ({counter}) < Packet size ({head_packet['size']}) - stopping this tick")
                break  # Exit the repeat loop
            
            # Step 1.1: Pop a packet out of the head of the queue
            packet = dequeue_packet()
            print(f"Step 1.1: Popped packet {packet['id']} (size {packet['size']}) from queue")
            
            # Step 1.2: Send the packet into the network
            print("Step 1.2: ", end="")
            send_packet(packet)
            
            # Step 1.3: Decrement the counter by the size of packet
            counter = counter - packet['size']
            print(f"Step 1.3: Decremented counter by {packet['size']}, new counter = {counter}")
            
            print(f"\n\nRemaining packets in queue: {len(packet_queue)}")
        
        # Step 2: Reset counter and go to step 1 (next clock tick)
        print("Step 2: Reset counter and wait for next clock tick")
        
        # Show status before next tick
        show_queue_status()
        
        tick += 1
        
        # Simulate clock tick delay
        time.sleep(2)
    
    print("\n=== Complete - All packets processed ===")

def main():
    # Add packets to the queue
    enqueue_packet(3, 101)  # Packet ID 101, size 3
    enqueue_packet(2, 102)  # Packet ID 102, size 2
    enqueue_packet(5, 103)  # Packet ID 103, size 5
    enqueue_packet(4, 104)  # Packet ID 104, size 4
    enqueue_packet(1, 105)  # Packet ID 105, size 1
    enqueue_packet(6, 106)  # Packet ID 106, size 6
    enqueue_packet(3, 107)  # Packet ID 107, size 3
    enqueue_packet(7, 108)  # Packet ID 108, size 7
    
    print("\nInitial ", end="")
    show_queue_status()
    
    # Run the leaky bucket algorithm
    leaky_bucket_algorithm()

if __name__ == "__main__":
    main()