#ifndef APPLICATIONS_CONFIG_H_
#define APPLICATIONS_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//List all implemented subscriber topic types
typedef enum {
    SubscriberTopic_None = 0,
    SubscriberTopic_Twist = 1
} SubscriberTopicTypes;

//List all implemented publisher topic types
typedef enum {
    PublisherTopic_None = 0,
    PublisherTopic_Twist = 1
} PublisherTopicTypes;

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_CONFIG_H_ */
