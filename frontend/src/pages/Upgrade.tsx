import { useEffect, useState } from "react";
import { type Account, getAccount } from "../services/api";
import { BusinessPlans, PersonalPlans } from "./upgrade/PlanSections";
import { UpgradeCta } from "./upgrade/UpgradeCta";
import { UpgradeHero } from "./upgrade/UpgradeHero";

export default function Upgrade() {
  const [account, setAccount] = useState<Account | null>(null);

  useEffect(() => {
    getAccount()
      .then(setAccount)
      .catch(() => setAccount(null));
  }, []);

  const isBusiness = account?.accountType === "BUSINESS";
  const currentPlan = account?.subscription.plan ?? "STANDARD";

  return (
    <div
      className="max-w-7xl
      mx-auto
      space-y-10"
    >
      <UpgradeHero
        account={account}
        isBusiness={isBusiness}
        currentPlan={currentPlan}
      />

      {isBusiness ? (
        <BusinessPlans currentPlan={currentPlan} />
      ) : (
        <PersonalPlans currentPlan={currentPlan} />
      )}

      <UpgradeCta />
    </div>
  );
}
