#ifndef NET_CODERODDE_UTIL_STABLE_PRIORITY_QUEUE
#define NET_CODERODDE_UTIL_STABLE_PRIORITY_QUEUE

#include <map>
#include <stdexcept>
#include <unordered_map>

namespace net {
namespace coderodde {
namespace util {
    
    template<typename T, typename Priority>
    class StablePriorityQueue {
    private:
        
        struct ElementList;
        
        struct ElementListNode {
            T m_element;
            ElementListNode* m_next;
            ElementListNode* m_prev;
            ElementList* m_owner_list;
            
            ElementListNode(T element, ElementList* owner_list)
            :
            m_element{element},
            m_owner_list{owner_list}
            {}
        };
        
        struct ElementList {
            Priority m_priority;
            ElementListNode* m_head;
            ElementListNode* m_tail;
            
            ElementList(Priority priority)
            :
            m_priority{priority},
            m_head{nullptr},
            m_tail{nullptr}
            {}
            
            void appendElementListNode(ElementListNode* element_list_node) {
                if (m_head == nullptr) {
                    m_head = element_list_node;
                    m_tail = element_list_node;
                    element_list_node->m_prev = nullptr;
                    element_list_node->m_next = nullptr;
                } else {
                    m_tail->m_next = element_list_node;
                    element_list_node->m_prev = m_tail;
                    element_list_node->m_next = nullptr;
                    m_tail = element_list_node;
                }
            }
            
            T removeHead() {
                T ret = m_head->m_element;
                ElementListNode* old_node = m_head;
                m_head = m_head->m_next;
                delete old_node;
                
                if (m_head != nullptr) {
                    m_head->m_prev = nullptr;
                }
                
                return ret;
            }
            
            bool isEmpty() {
                return m_head == nullptr;
            }
        };
        
    public:
        
        ~StablePriorityQueue() {
            for (std::pair<Priority, ElementList*> p : m_priority_map) {
                delete p.second;
            }
            
            for (std::pair<T, ElementListNode*> p : m_element_map) {
                delete p.second;
            }
        }
        
        void add(T element, Priority priority) {
            if (m_element_map.find(element) == m_element_map.cend()) {
                addNewEntry(element, priority);
            } else {
                updateEntry(m_element_map[element], priority);
            }
        }
        
        T top() {
            checkQueueNotEmpty();
            Priority priority = (*m_priority_map.cbegin()).first;
            return m_priority_map[priority]->m_head->m_element;
        }
        
        T extractMinimum()  {
            checkQueueNotEmpty();
            Priority priority = (*m_priority_map.cbegin()).first;
            ElementList* element_list = m_priority_map[priority];
            T result = element_list->removeHead();
            
            if (element_list->isEmpty()) {
                m_priority_map.erase(element_list->m_priority);
                delete element_list;
            }
            
            m_element_map.erase(result);
            return result;
        }
        
        bool empty() {
            return m_element_map.empty();
        }
        
        size_t size() {
            return m_element_map.size();
        }
        
    private:
        std::map<Priority, ElementList*> m_priority_map;
        std::unordered_map<T, ElementListNode*> m_element_map;
        
    private:
        
        void addNewEntry(T element, Priority priority) {
            ElementList* element_list = nullptr;
            
            if (m_priority_map.find(priority) != m_priority_map.cend()) {
                element_list = m_priority_map[priority];
            }
            
            if (element_list == nullptr) {
                element_list = new ElementList(priority);
                m_priority_map[priority] = element_list;
            }
            
            ElementListNode* element_list_node = new ElementListNode(
                                                            element,
                                                            element_list);
            
            element_list->appendElementListNode(element_list_node);
            m_element_map[element] = element_list_node;
        }
        
        void updateEntry(ElementListNode* element_list_node,
                         Priority new_priority) {
            unlink(element_list_node);
            relink(element_list_node, new_priority);
        }
        
        void unlink(ElementListNode* element_list_node) {
            ElementList* element_list = element_list_node->m_owner_list;
            
            if (element_list_node->m_prev != nullptr) {
                element_list_node->m_prev->m_next = element_list_node->m_next;
            } else {
                element_list->m_head = element_list->m_head->m_next;
                
                if (element_list->m_head != nullptr) {
                    element_list->m_head->m_prev = nullptr;
                }
            }
            
            if (element_list_node->m_next != nullptr) {
                element_list_node->m_next->m_prev = element_list_node->m_prev;
            } else {
                element_list->m_tail = element_list->m_tail->m_prev;
                
                if (element_list->m_tail != nullptr) {
                    element_list->m_tail->m_next = nullptr;
                }
            }
            
            if (element_list->isEmpty()) {
                m_priority_map.erase(element_list->m_priority);
                delete element_list;
            }
        }
        
        void relink(ElementListNode* element_list_node, Priority priority) {
            ElementList* element_list = nullptr;
            
            if (m_priority_map.find(priority) != m_priority_map.cend()) {
                element_list = m_priority_map[priority];
            }
            
            if (element_list == nullptr) {
                element_list = new ElementList(priority);
                m_priority_map[priority] = element_list;
            }
            
            element_list->appendElementListNode(element_list_node);
        }
        
        void checkQueueNotEmpty() {
            if (empty()) {
                throw std::underflow_error{"The priority queue is empty."};
            }
        }
    };
    
} // End of namespace net::coderodde::util.
} // End of namespace net::coderodde.
} // End of namespace net.

#endif // NET_CODERODDE_UTIL_STABLE_PRIORITY_QUEUE
