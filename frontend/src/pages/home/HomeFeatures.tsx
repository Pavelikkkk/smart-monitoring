const features = [
  {
    id: "features",
    title: "Real-Time Monitoring",
    description:
      "ESP32 devices stream live power, motion and light measurements through MQTT.",
    image: "https://images.unsplash.com/photo-1518770660439-4636190af475",
    alt: "IoT Monitoring",
    reverse: false,
  },
  {
    title: "Hybrid Detection",
    description: "Rule-based and ONNX-powered machine learning anomaly detection.",
    image:
      "https://avatars.mds.yandex.net/i?id=9cb64b10b0a2964b06d946de60c6e61a_l-12651048-images-thumbs&n=13",
    alt: "Smart Building",
    reverse: true,
  },
  {
    title: "Analytics Platform",
    description:
      "Historical trends, room statistics, anomaly reports and energy insights.",
    image: "https://images.unsplash.com/photo-1460925895917-afdab827c52f",
    alt: "Analytics Dashboard",
    reverse: false,
  },
];

function FeatureImage({ image, alt }: { image: string; alt: string }) {
  return (
    <div
      className="h-96
      bg-[#111827]
      rounded-3xl
      border
      border-cyan-700/40
      flex
      items-center
      justify-center"
    >
      <img
        src={image}
        alt={alt}
        className="w-full
        h-96
        object-cover
        rounded-3xl"
      />
    </div>
  );
}

function FeatureCopy({
  title,
  description,
}: {
  title: string;
  description: string;
}) {
  return (
    <div>
      <h2
        className="text-5xl
        font-bold
        mb-6"
      >
        {title}
      </h2>

      <p
        className="text-xl
        text-slate-300"
      >
        {description}
      </p>
    </div>
  );
}

export function HomeFeatures() {
  return (
    <>
      {features.map((feature) => (
        <section
          key={feature.title}
          id={feature.id}
          className="grid
          grid-cols-1
          xl:grid-cols-2
          gap-12
          py-24
          items-center"
        >
          {feature.reverse ? (
            <>
              <div
                className="order-2
                xl:order-1"
              >
                <FeatureImage
                  image={feature.image}
                  alt={feature.alt}
                />
              </div>

              <div
                className="order-1
                xl:order-2"
              >
                <FeatureCopy
                  title={feature.title}
                  description={feature.description}
                />
              </div>
            </>
          ) : (
            <>
              <FeatureCopy
                title={feature.title}
                description={feature.description}
              />

              <FeatureImage
                image={feature.image}
                alt={feature.alt}
              />
            </>
          )}
        </section>
      ))}
    </>
  );
}
