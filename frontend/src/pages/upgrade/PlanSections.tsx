type Plan = {
  name: string;
  price: string;
  badge?: "Current" | "Popular" | "Best Choice";
  highlight?: boolean;
  currentWhen?: string;
  features: string[];
};

const personalPlans: Plan[] = [
  {
    name: "Free",
    price: "Free",
    badge: "Current",
    highlight: true,
    features: ["вњ… 2 Rooms", "вњ… 5 Devices", "вњ… Anomalies", "вќЊ Analytics"],
  },
  {
    name: "Basic",
    price: "$4.99 / month",
    features: [
      "вњ… 10 Rooms",
      "вњ… 30 Devices",
      "вњ… Notifications",
      "вњ… Full History",
    ],
  },
  {
    name: "Pro",
    price: "$9.99 / month",
    badge: "Popular",
    highlight: true,
    features: [
      "вњ… 50 Rooms",
      "вњ… 100 Devices",
      "вњ… AI Analytics",
      "вњ… Priority Support",
    ],
  },
];

const businessPlans: Plan[] = [
  {
    name: "Free",
    price: "Free",
    currentWhen: "FREE",
    features: [
      "вњ… 1 Building",
      "вњ… 5 Rooms",
      "вњ… 10 Devices",
      "вќЊ Advanced Analytics",
    ],
  },
  {
    name: "Standard",
    price: "$29.99 / month",
    badge: "Best Choice",
    currentWhen: "STANDARD",
    features: ["вњ… 5 Buildings", "вњ… 100 Rooms", "вњ… 500 Devices", "вњ… Analytics"],
  },
  {
    name: "Enterprise",
    price: "Contact Sales",
    currentWhen: "ENTERPRISE",
    features: [
      "вњ… Unlimited Buildings",
      "вњ… Unlimited Rooms",
      "вњ… Unlimited Devices",
      "вњ… Team Management",
      "вњ… Reports",
      "вњ… Dedicated Support",
    ],
  },
];

function Badge({ children }: { children: React.ReactNode }) {
  return (
    <div
      className="inline-block
      px-4
      py-2
      rounded-xl
      bg-cyan-500
      text-slate-900
      font-semibold
      mb-4"
    >
      {children}
    </div>
  );
}

function PlanCard({ plan, currentPlan }: { plan: Plan; currentPlan: string }) {
  const isCurrent = plan.currentWhen
    ? currentPlan === plan.currentWhen
    : plan.badge === "Current";
  const borderClass =
    isCurrent || plan.highlight ? "border-cyan-500" : "border-cyan-700/40";

  return (
    <div
      className={`bg-[#111827]
      rounded-3xl
      border
      ${borderClass}
      p-5`}
    >
      <div
        className="flex
        items-center
        gap-3
        mb-4"
      >
        {plan.badge && (
          <Badge>
            {plan.badge}
          </Badge>
        )}

        {isCurrent && plan.badge !== "Current" && <Badge>Current</Badge>}
      </div>

      <h3
        className="text-3xl
        font-bold"
      >
        {plan.name}
      </h3>

      <div
        className="text-cyan-400
        mt-2"
      >
        {plan.price}
      </div>

      <div
        className="space-y-3
        mt-8"
      >
        {plan.features.map((feature) => (
          <div key={feature}>{feature}</div>
        ))}
      </div>
    </div>
  );
}

function PlanSection({
  title,
  plans,
  currentPlan,
}: {
  title: string;
  plans: Plan[];
  currentPlan: string;
}) {
  return (
    <div>
      <h2
        className="text-4xl
        font-bold
        mb-6"
      >
        {title}
      </h2>

      <div
        className="grid
        grid-cols-1
        lg:grid-cols-3
        gap-5"
      >
        {plans.map((plan) => (
          <PlanCard
            key={plan.name}
            plan={plan}
            currentPlan={currentPlan}
          />
        ))}
      </div>
    </div>
  );
}

export function PersonalPlans({ currentPlan }: { currentPlan: string }) {
  return (
    <PlanSection
      title="рџ‘¤ Personal Plans"
      plans={personalPlans}
      currentPlan={currentPlan}
    />
  );
}

export function BusinessPlans({ currentPlan }: { currentPlan: string }) {
  return (
    <PlanSection
      title="рџЏў Business Plans"
      plans={businessPlans}
      currentPlan={currentPlan}
    />
  );
}
